#include "engine.h"

#include "global.h"

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

  void RenderLoop(float   scale,
                  int     win_width,
                  int     win_height,
                  color_t col_bg,
                  bool    resizable) {
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

    // camera
    scene.camera.configure({
      { "aspect", window.aspect() },
      { "position", pos_t(1.5f, 1.5f, 2.0f) },
      { "type", camera::CameraType::Perspective }
    });
    glfwSetWindowUserPointer(window.window(), &scene.camera);
    glfwSetCursorPosCallback(window.window(),
                             camera::Camera::mouseInputCallback);

    // lights
    auto point_light   = light::Point({
      { "position", pos_t(1.5f, 1.5f, 2.0f) * 2.0f },
      { "ambientStrength", 0.1f },
      { "diffuseStrength", 0.5f },
      { "specularStrength", 1.0f },
      { "specularColor", color_t(1.0f, 0.3f, 0.8f) }
    });
    auto distant_light = light::Distant({
      { "direction", pos_t(-0.2f, -1.0f, -0.3f) },
      { "ambientStrength", 0.0f },
      { "diffuseStrength", 0.0f },
      { "specularStrength", 0.7f }
    });
    auto spot_light    = light::Spotlight({
      { "ambientStrength", 0.0f },
      { "diffuseStrength", 1.0f },
      { "specularStrength", 1.0f },
      { "diffuseColor", color_t(0.5f, 0.5f, 1.0f) }
    });

    // materials

    // meshes
    auto cube = mesh::Mesh("cube", prefabs::Cube());
    cube.regenBuffers();

    scene.addMaterial(new material::Default(
      "cube material",
      {
        {      "shininess",      128.0f                           },
        { "diffuseTexture", exe_path / "assets" / "container2.png"},
        {"specularTexture",
         exe_path / "assets" / "container2_specular.png"          }
    }));
    scene.addMesh(&cube);
    scene.addLightMesh(&cube);
    scene.addLight(&point_light);
    scene.addLight(&distant_light);
    scene.addLight(&spot_light);

    cube.attachMaterial(scene.material(0));

    scene.configureShaders();
    scene.compileShaders();
    scene.print();

    log::log(log::INFO, "starting render loop");

    timer::Ticker ticker;
    while (!window.windowShouldClose()) {
      ticker.tick();
      const auto new_pos = pos_t(1.0f * glm::cos(ticker.time()),
                                 2.0f,
                                 1.0f * glm::sin(ticker.time()));
      spot_light.configure({
        { "position",               new_pos},
        {"direction", pos_t(0.0f) - new_pos}
      });

      window.processKeyboardInput();
      scene.camera.processKeyboardInput(window.window(), ticker.dt());

      window.clear();
      scene.render(0, ticker.time());

      glfwSwapBuffers(window.window());
      glfwPollEvents();
    }
  }

} // namespace engine
