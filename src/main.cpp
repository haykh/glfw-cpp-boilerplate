#include "api/engine.h"
#include "utils/error.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace raise = utils;

auto main(int, char**) -> int {
  if (glfwInit()) {
    try {
      engine::RenderLoop();
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
