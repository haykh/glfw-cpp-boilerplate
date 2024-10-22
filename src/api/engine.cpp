#include "engine.h"

#include "api/camera.h"
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

    static bool         firstMouse = true;
    static float        lastX      = 0.0f;
    static float        lastY      = 0.0f;
    static api::Camera* cam        = &camera;

    auto mouse_callback = [](GLFWwindow*, double xPos, double yPos) {
      if (firstMouse) {
        firstMouse = false;
        lastX      = xPos;
        lastY      = yPos;
      }
      float xOffset = (xPos - lastX) * cam->Sensitivity;
      float yOffset = (lastY - yPos) * cam->Sensitivity;
      cam->setYaw(cam->yaw() + xOffset);
      cam->setPitch(cam->pitch() + yOffset);
      lastX = xPos;
      lastY = yPos;
    };

    glfwMakeContextCurrent(window.window());

    glfwSetFramebufferSizeCallback(window.window(), framebuffer_size_callback);
    glfwSetCursorPosCallback(window.window(), mouse_callback);
    glfwSetInputMode(window.window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL(glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

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

    utils::Ticker ticker;
    while (!window.windowShouldClose()) {
      ticker.tick();

      window.processInput();
      camera.processInput(window.window(), ticker.dt());
      camera.pointAt(glm::vec3(0.0f, 0.0f, 0.0f));

      window.clear();

      shader.use();
      shader.setUniform1f("time", ticker.time());

      // cube.setTransform(
      //   glm::translate(glm::mat4(1.0f),
      //                  glm::vec3(0.0f, 0.0f, sin(ticker.time()))));

      shader.render({ &cube }, camera);

      window.unuse();
    }
  }

} // namespace engine
