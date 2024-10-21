#include "api/engine.h"
#include "utils/error.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace raise = utils;

auto main(int argc, char* argv[]) -> int {
  if (glfwInit()) {
    try {
      auto engine = engine::Engine();
      engine.renderLoop();
      glfwTerminate();
    } catch (const std::exception& e) {
      glfwTerminate();
      raise::error(e.what());
    }
  } else {
    raise::error("Failed to initialize GLFW");
  }
  return 0;
}
