#include "engine.h"

#include "api/camera.h"
#include "api/mesh.h"
#include "api/prefabs.h"
#include "api/shader.h"
#include "api/window.h"
#include "utils/log.h"
#include "utils/paths.h"

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine {
  namespace log  = utils;
  namespace path = utils;

  Engine::Engine(float scale,
                 int   win_width,
                 int   win_height,
                 float col_bg[4],
                 bool  resizable)
    : m_window { std::make_unique<api::Window>((int)(win_width * scale / 2.0f),
                                               (int)(win_height * scale / 2.0f),
                                               "engine_window",
                                               col_bg,
                                               resizable) } {}

  void Engine::renderLoop() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    api::Camera camera;
    camera.setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
    camera.setAspect(m_window->aspect());
    camera.setStare(true);

    api::ShaderProgram shader("rectangle");
    const auto         exe_path = path::exeDir();
    shader.readShadersFromPaths(
      (exe_path / "shaders" / "test.vert").generic_string(),
      (exe_path / "shaders" / "test.frag").generic_string());
    shader.compile();
    shader.link();

    auto cube = api::Mesh("cube", prefabs::Cube());
    cube.genBuffers();
    cube.bind();

    log::log(log::INFO, "starting render loop");
    while (!m_window->windowShouldClose()) {
      m_window->processInput();
      m_window->clear();

      // render stuff here >>>
      shader.use();
      shader.setUniform1f("time", (float)glfwGetTime());

      cube.setTransform(
        glm::translate(glm::mat4(1.0f),
                       glm::vec3(0.0f, 0.0f, sin(glfwGetTime()))));
      camera.setPosition(glm::vec3(3.0f * sin(0.2f * glfwGetTime()),
                                   0.0f,
                                   3.0f * cos(0.2f * glfwGetTime())));

      shader.render({ &cube }, camera);
      // finish rendering stuff <<<

      m_window->unuse();
    }
  }

} // namespace engine
