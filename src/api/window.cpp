#include "window.h"

#include "utils/error.h"
#include "utils/log.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace engine {

  Window::Window(int                width,
                 int                height,
                 const std::string& name,
                 int                swapInterval,
                 bool               isResizable)
    : m_winWidth(width)
    , m_winHeight(height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (!isResizable) {
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    m_win = glfwCreateWindow(m_winWidth,
                             m_winHeight,
                             name.c_str(),
                             nullptr,
                             nullptr);
    if (m_win == nullptr) {
      raise::error("Failed to open window");
    } else {
      log::log(log::SUCCESS, "Window opened");
    }
    glfwMakeContextCurrent(m_win);
    glfwSwapInterval(swapInterval);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      raise::error("Failed to load GLAD");
    } else {
      log::log(log::SUCCESS, "GLAD loaded");
    }
  }

  Window::~Window() {
    glfwDestroyWindow(m_win);
  }

  void Window::processInput() {
    if (glfwGetKey(m_win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(m_win, true);
    }
  }

  void Window::unuse() {
    glfwSwapBuffers(m_win);
    glfwPollEvents();
  }

} // namespace engine