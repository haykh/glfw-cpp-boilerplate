#include "scene.h"

#include "api/light.h"
#include "api/material.h"
#include "api/mesh.h"
#include "utils/log.h"

#include <glad/gl.h>

#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <filesystem>
#include <map>
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
      m_meshes.push_back(new mesh::Mesh("lightsource", prefabs::Cube()));
      m_meshes.back()->regenBuffers();
      auto emitter = new material::Emitter("lightsource material");
      emitter->setColor(p_light->diffuseStrength() > p_light->specularStrength()
                          ? p_light->diffuseColor()
                          : p_light->specularColor());
      m_meshes.back()->setMaterial(emitter);
      m_meshes.back()->bindPosition(
        dynamic_cast<Positional*>(p_light)->position_ptr);
      m_meshes.back()->setScale(vec_t { 0.1f });
    }
  }

  void Scene::configureShaders() {
    for (auto& shader : m_shaders) {
      std::string light_declarations    = "";
      std::string light_calls           = "";
      std::string material_declarations = "";
      for (const auto& light : m_lights) {
        light_declarations += light->shaderDeclaration() + "\n";
        light_calls        += "\n    " + light->shaderCall();
      }
      shader.fragmentShader().replaceString("/* subst: light sources */",
                                            light_declarations);
      shader.fragmentShader().replaceString("/* subst: light calculations */",
                                            light_calls);
      std::map<unsigned int, unsigned int> number_of_materials;
      for (const auto& mesh : m_meshes) {
        const auto inShaderId = mesh->material()->inShaderId();
        if (number_of_materials.find(inShaderId) == number_of_materials.end()) {
          number_of_materials[inShaderId] = 1;
        } else {
          number_of_materials[inShaderId]++;
        }
      }
      for (const auto& [id, count] : number_of_materials) {
        material_declarations += api::material::shaderDeclaration(id, count) +
                                 "\n";
      }
      shader.fragmentShader().replaceString("/* subst: materials */",
                                            material_declarations);
    }
  }

  void Scene::compileShaders() {
    for (auto& shader : m_shaders) {
      shader.compile();
      shader.link();
    }
  }

  void Scene::addShader(const std::string&           shader_name,
                        const std::filesystem::path& shader_path) {
    m_shaders.emplace_back(shader_name);
    m_shaders.back().readShadersFromPaths(
      (shader_path / shader_name).generic_string() + ".vert.in",
      (shader_path / shader_name).generic_string() + ".frag.in");
  }

  void Scene::render(unsigned int shader, float time) const {
    const auto activeShader = m_shaders[shader];
    activeShader.use();
    activeShader.setUniform1f("time", time);
    for (const auto& light : m_lights) {
      if (light == nullptr) {
        log::log(log::WARNING, "light source is null");
      } else {
        light->illuminate(activeShader);
      }
    }
    activeShader.setUniform3fv("camera.position", camera.position());
    activeShader.setUniformMatrix4fv("camera.view", camera.view());
    activeShader.setUniformMatrix4fv("camera.projection", camera.project());
    glBindVertexArray(m_vao);
    for (const auto& mesh : m_meshes) {
      if (mesh == nullptr) {
        log::log(log::WARNING, "mesh is null");
      } else {
        activeShader.setUniformMatrix4fv("model", mesh->transform());
        mesh->material()->shade(activeShader);
        mesh->render(activeShader);
      }
    }
    glBindVertexArray(0);
  }

  // void Scene::renderLights() const {
  //   m_light_shader.use();
  //   m_light_shader.setUniformMatrix4fv("view", camera.view());
  //   m_light_shader.setUniformMatrix4fv("projection", camera.project());
  //   glBindVertexArray(m_light_vao);
  //   for (const auto& light : m_positional_lights) {
  //     if (light == nullptr) {
  //       log::log(log::WARNING, "light source is null");
  //     } else {
  //       auto model = transform_t(1.0f);
  //       model      = glm::translate(model, light->position());
  //       model      = glm::scale(model, vec_t(0.1f));
  //       m_light_shader.setUniformMatrix4fv("model", model);
  //       if (light->diffuseStrength() > light->specularStrength()) {
  //         m_light_shader.setUniform3fv("modelColor", light->diffuseColor());
  //       } else {
  //         m_light_shader.setUniform3fv("modelColor", light->specularColor());
  //       }
  //       m_light_mesh->render(m_light_shader);
  //     }
  //   }
  //   glBindVertexArray(0);
  // }

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
