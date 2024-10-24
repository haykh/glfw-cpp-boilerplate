#include "engine.h"

#include "api/light.h"
#include "api/mesh.h"
#include "api/prefabs.h"
#include "api/scene.h"
#include "api/window.h"
#include "utils/log.h"
#include "utils/paths.h"
#include "utils/ticker.h"

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace engine {
  using namespace utils;
  using namespace api;
  using pos_t = glm::vec3;
  using col_t = glm::vec3;

  void RenderLoop(float scale,
                  int   win_width,
                  int   win_height,
                  float col_bg[4],
                  bool  resizable) {
    window::Window window { (int)(win_width * scale / 2.0f),
                            (int)(win_height * scale / 2.0f),
                            "engine_window",
                            col_bg,
                            resizable };
    glfwMakeContextCurrent(window.window());
    gladLoadGL(glfwGetProcAddress);
    auto framebuffer_size_callback = [](GLFWwindow*, int width, int height) {
      glViewport(0, 0, width, height);
    };
    glfwSetFramebufferSizeCallback(window.window(), framebuffer_size_callback);
    glfwSetInputMode(window.window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // scene setup
    scene::Scene scene;
    // shader setup
    const auto   exe_path = path::exeDir();
    scene.addShader("example", exe_path / "shaders");
    scene.addLightShader(exe_path / "shaders");

    // camera setup
    scene.camera.setAspect(window.aspect());
    scene.camera.setFOV(90.0f);
    scene.camera.setPosition({ 1.5f, 1.5f, 2.0f });
    scene.camera.pointAt({ 0.0f, 0.0f, 0.0f });
    glfwSetWindowUserPointer(window.window(), &scene.camera);
    glfwSetCursorPosCallback(window.window(),
                             camera::Camera::mouseInputCallback);

    auto cube = mesh::Mesh("cube", prefabs::Cube());
    cube.setColor({ 1.0f, 1.0f, 1.0f });
    cube.regenBuffers();

    scene.addMesh(&cube);
    scene.addLightMesh(&cube);

    auto point_light = light::Point();
    point_light.setPosition(glm::vec3(1.5f, 1.5f, 2.0f) * 2.0f);
    point_light.setAmbientStrength(0.0f);
    point_light.setDiffuseStrength(0.3f);
    point_light.setSpecularStrength(0.7f);
    point_light.setSpecularColor({ 1.0f, 0.3f, 0.8f });
    point_light.setAttenuation(1.0f, 0.09f, 0.032f);

    auto distant_light = light::Distant();
    distant_light.setDirection({ -0.2f, -1.0f, -0.3f });
    distant_light.setAmbientStrength(0.0f);
    distant_light.setDiffuseStrength(0.0f);
    distant_light.setSpecularStrength(0.7f);

    scene.addLight(&point_light);
    scene.addLight(&distant_light);

    scene.print();

    log::log(log::INFO, "starting render loop");

    timer::Ticker ticker;
    while (!window.windowShouldClose()) {
      ticker.tick();

      window.processKeyboardInput();
      scene.camera.processKeyboardInput(window.window(), ticker.dt());

      window.clear();
      scene.render(0, ticker.time());
      scene.renderLights();

      glfwSwapBuffers(window.window());
      glfwPollEvents();
    }
  }

} // namespace engine
