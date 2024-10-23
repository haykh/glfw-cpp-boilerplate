#include "engine.h"

#include "api/camera.h"
#include "api/light.h"
#include "api/mesh.h"
#include "api/prefabs.h"
#include "api/shader.h"
#include "api/window.h"
#include "utils/log.h"
#include "utils/paths.h"
#include "utils/ticker.h"

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine {
  namespace log  = utils;
  namespace path = utils;

  void RenderLoop(float scale,
                  int   win_width,
                  int   win_height,
                  float col_bg[4],
                  bool  resizable) {
    api::Window window { (int)(win_width * scale / 2.0f),
                         (int)(win_height * scale / 2.0f),
                         "engine_window",
                         col_bg,
                         resizable };
    api::Camera camera;
    camera.setAspect(window.aspect());
    camera.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    auto framebuffer_size_callback = [](GLFWwindow*, int width, int height) {
      glViewport(0, 0, width, height);
    };

    glfwSetWindowUserPointer(window.window(), &camera);
    glfwMakeContextCurrent(window.window());

    glfwSetFramebufferSizeCallback(window.window(), framebuffer_size_callback);
    glfwSetCursorPosCallback(window.window(), api::Camera::mouseInputCallback);
    glfwSetInputMode(window.window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL(glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    const auto exe_path = path::exeDir();
    api::Light light { (exe_path / "shaders" / "light.vert").generic_string(),
                       (exe_path / "shaders" / "light.frag").generic_string() };

    auto cube = api::Mesh("cube", prefabs::Cube());

    light.bind(cube.vbo());

    log::log(log::INFO, "starting render loop");

    utils::Ticker ticker;
    while (!window.windowShouldClose()) {
      ticker.tick();

      window.processKeyboardInput();
      camera.processKeyboardInput(window.window(), ticker.dt());

      window.clear();
      light.render({ &cube }, camera, ticker.time());
      // shader.use();
      // shader.setUniform1f("time", ticker.time());
      // shader.render({ &cube }, camera);

      glfwSwapBuffers(window.window());
      glfwPollEvents();
    }
  }

} // namespace engine
