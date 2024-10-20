#include "engine.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace engine {

  Engine::Engine(float scale, int win_width, int win_height, bool resizable) {
    m_window = std::make_unique<Window>((int)(win_width * scale / 2.0f),
                                        (int)(win_height * scale / 2.0f),
                                        "engine_window",
                                        1,
                                        resizable);
  }

  void Engine::renderLoop() {
    while (!m_window->windowShouldClose()) {
      m_window->processInput();
      glfwPollEvents();

      // ... render stuff here

      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      m_window->unuse();
    }
  }

} // namespace engine
