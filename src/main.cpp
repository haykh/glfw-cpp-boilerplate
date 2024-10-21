#include "api/engine.h"
#include "utils/error.h"

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
