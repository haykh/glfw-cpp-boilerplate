#include "engine.h"

#include "api/mesh.h"
#include "api/prefabs.h"
#include "api/shader.h"
#include "utils/log.h"

#include <GL/gl.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {
  namespace log = utils;

  Engine::Engine(float scale,
                 int   win_width,
                 int   win_height,
                 float col_bg[4],
                 bool  resizable) {
    m_window = std::make_unique<api::Window>((int)(win_width * scale / 2.0f),
                                             (int)(win_height * scale / 2.0f),
                                             "engine_window",
                                             1,
                                             col_bg,
                                             resizable);
  }

  void Engine::renderLoop() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            m_window->aspect(),
                                            0.1f,
                                            100.0f);

    api::ShaderProgram shader("rectangle");
    shader.readShadersFromPaths(
      "/home/hayk/Tinkering/glfw/src/shaders/test.vert",
      "/home/hayk/Tinkering/glfw/src/shaders/test.frag");
    shader.compile();
    shader.link();

    auto mesh = api::Mesh("cube", prefabs::Cube());
    mesh.genBuffers();
    mesh.bind();

    log::log(log::INFO, "starting render loop");
    while (!m_window->windowShouldClose()) {
      m_window->processInput();
      m_window->clear();

      // render stuff here >>>
      shader.use();
      glUniformMatrix4fv(glGetUniformLocation(shader.id(), "model"),
                         1,
                         GL_FALSE,
                         glm::value_ptr(model));
      glm::mat4 view = glm::mat4(1.0f);
      view           = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
      view           = glm::rotate(view,
                         glm::radians(-70.0f),
                         glm::vec3(1.0f, 0.0f, 0.0f));
      view           = glm::rotate(view,
                         (float)glfwGetTime(),
                         glm::vec3(0.0f, 0.0f, 1.0f));
      glUniformMatrix4fv(glGetUniformLocation(shader.id(), "view"),
                         1,
                         GL_FALSE,
                         glm::value_ptr(view));
      glUniformMatrix4fv(glGetUniformLocation(shader.id(), "projection"),
                         1,
                         GL_FALSE,
                         glm::value_ptr(projection));

      shader.setUniform1f("time", (float)glfwGetTime());
      mesh.render();
      // finish rendering stuff <<<

      m_window->unuse();
    }
  }

} // namespace engine
