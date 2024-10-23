#include "light.h"

#include "api/mesh.h"

#include <glad/gl.h>

#include <glm/glm.hpp>

#include <string>

namespace api {

  Light::Light(const std::string& vert,
               const std::string& frag,
               float              A,
               float              D,
               const glm::vec3&   col,
               const glm::vec3&   pos)
    : m_ambient_strength { A }
    , m_diffuse_strength { D }
    , m_color { col }
    , m_position { pos }
    , shader { "light" } {
    shader.readShadersFromPaths(vert, frag);
    shader.compile();
    shader.link();
    glGenVertexArrays(1, &m_vao);
  }

  void Light::bind(unsigned int vbo) const {
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
  }

  void Light::render(const std::vector<const Mesh*>& meshes,
                     const Camera&                   camera,
                     float                           time) const {
    shader.use();
    shader.setUniform1f("time", time);
    shader.setUniform1f("ambientStrength", m_ambient_strength);
    shader.setUniform1f("diffuseStrength", m_diffuse_strength);
    shader.setUniform3f("lightColor", m_color);
    shader.setUniform3f("lightPos", m_position);
    shader.setUniformMatrix4fv("view", camera.view());
    shader.setUniformMatrix4fv("projection", camera.project());
    for (const auto& mesh : meshes) {
      shader.setUniformMatrix4fv("model", mesh->transform());
      shader.setUniform3f("objectColor", mesh->color());
      mesh->render();
    }
  }

} // namespace api
