#include "scene.h"

#include "api/light.h"
#include "api/mesh.h"
#include "utils/log.h"

#include <glad/gl.h>

#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <filesystem>
#include <string>

namespace api::scene {
  using namespace api::shader;
  using namespace api::light;
  using namespace utils;

  Scene::Scene() : m_light_shader { "lightsource" } {
    glGenVertexArrays(1, &m_vao);
    glGenVertexArrays(1, &m_light_vao);
  }

  Scene::~Scene() {
    glDeleteVertexArrays(1, &m_light_vao);
    glDeleteVertexArrays(1, &m_vao);
  }

  void Scene::addMesh(Mesh* p_mesh) {
    m_meshes.push_back(p_mesh);
    glBindVertexArray(m_vao);
    {
      glBindBuffer(GL_ARRAY_BUFFER, p_mesh->vbo());
      {
        // position attribute
        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              6 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);
        // normal attribute
        glVertexAttribPointer(1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              6 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
      }
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);
  }

  void Scene::addLightMesh(Mesh* mesh) {
    m_light_mesh = mesh;
    glBindVertexArray(m_light_vao);
    {
      glBindBuffer(GL_ARRAY_BUFFER, m_light_mesh->vbo());
      {
        // position attribute
        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              6 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);
      }
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);
  }

  void Scene::addLight(LightSource* p_light) {
    m_lights.push_back(p_light);
    if (p_light->type() != LightType::Distant) {
      m_positional_lights.push_back(dynamic_cast<Positional*>(p_light));
    }
    std::string shader_declarations = "";
    std::string shader_calls        = "";
    for (const auto& light : m_lights) {
      shader_declarations += light->shaderDeclaration() + "\n";
      shader_calls        += light->shaderCall() + "\n";
    }
    for (auto& shader : m_shaders) {
      auto frag_src = shader.fragmentShader().original_source();
      auto pos      = frag_src.find("/* subst: light sources */");
      if (pos != std::string::npos) {
        frag_src.replace(pos, 26, shader_declarations);
      } else {
        log::log(
          log::WARNING,
          "could not find /* subst: light sources */ in fragment shader");
      }
      pos = frag_src.find("/* subst: light calculations */");
      if (pos != std::string::npos) {
        frag_src.replace(pos, 31, shader_calls);
      } else {
        log::log(
          log::WARNING,
          "could not find /* subst: light calculations */ in fragment shader");
      }
      shader.fragmentShader().setShaderSource(frag_src);
      shader.recompile();
      shader.relink();
    }
  }

  void Scene::addShader(const std::string&           shader_name,
                        const std::filesystem::path& shader_path) {
    m_shaders.emplace_back(shader_name);
    m_shaders.back().readShadersFromPaths(
      (shader_path / shader_name).generic_string() + ".vert",
      (shader_path / shader_name).generic_string() + ".frag");
    m_shaders.back().compile();
    m_shaders.back().link();
  }

  void Scene::addLightShader(const std::filesystem::path& shader_path) {
    m_light_shader.readShadersFromPaths(
      (shader_path / m_light_shader.label()).generic_string() + ".vert",
      (shader_path / m_light_shader.label()).generic_string() + ".frag");
    m_light_shader.compile();
    m_light_shader.link();
  }

  void Scene::render(unsigned int shader, float time) const {
    m_shaders[shader].use();
    m_shaders[shader].setUniform1f("time", time);
    for (const auto& light : m_lights) {
      if (light != nullptr) {
        light->illuminate(m_shaders[shader]);
      } else {
        log::log(log::WARNING, "light source is null");
      }
    }
    m_shaders[shader].setUniform3fv("camera.position", camera.position());
    m_shaders[shader].setUniformMatrix4fv("camera.view", camera.view());
    m_shaders[shader].setUniformMatrix4fv("camera.projection",
                                          camera.project());
    glBindVertexArray(m_vao);
    for (const auto& mesh : m_meshes) {
      if (mesh != nullptr) {
        m_shaders[shader].setUniformMatrix4fv("model", mesh->transform());
        m_shaders[shader].setUniform3fv("modelColor", mesh->color());
        mesh->render();
      } else {
        log::log(log::WARNING, "mesh is null");
      }
    }
    glBindVertexArray(0);
  }

  void Scene::renderLights() const {
    m_light_shader.use();
    m_light_shader.setUniformMatrix4fv("view", camera.view());
    m_light_shader.setUniformMatrix4fv("projection", camera.project());
    glBindVertexArray(m_light_vao);
    for (const auto& light : m_positional_lights) {
      if (light == nullptr) {
        log::log(log::WARNING, "light source is null");
      } else {
        auto model = glm::mat4(1.0f);
        model      = glm::translate(model, light->position());
        model      = glm::scale(model, glm::vec3(0.1f));
        m_light_shader.setUniformMatrix4fv("model", model);
        if (light->diffuseStrength() > light->specularStrength()) {
          m_light_shader.setUniform3fv("modelColor", light->diffuseColor());
        } else {
          m_light_shader.setUniform3fv("modelColor", light->specularColor());
        }
        m_light_mesh->render();
      }
    }
    glBindVertexArray(0);
  }

  void Scene::print() const {
    printf("\n..................\n");
    printf("Scene:\n");
    printf("  Camera:\n    ");
    camera.print();
    printf("\n");
    printf("  Meshes:\n");
    for (const auto& mesh : m_meshes) {
      printf("    ");
      mesh->print();
      printf("\n");
    }
    printf("  Lights:\n");
    for (const auto& light : m_lights) {
      printf("    ");
      light->print();
      printf("\n");
    }
    printf("  Shaders:\n");
    for (const auto& shader : m_shaders) {
      printf("    ");
      shader.print();
      printf("\n");
    }
    printf("\n..................\n");
  }

} // namespace api::scene
