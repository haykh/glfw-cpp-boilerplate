#include "window.h"

#include "global.h"

#include "utils/error.h"
#include "utils/log.h"

#include <glad/gl.h>

#include <GLFW/glfw3.h>

namespace api::window {
  using namespace utils;

  Window::Window(int                width,
                 int                height,
                 const std::string& name,
                 const color_t&     col_bg,
                 bool               isResizable)
    : m_winWidth(width)
    , m_winHeight(height)
    , m_col_bg { col_bg } {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (!isResizable) {
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_SAMPLES, 4);

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
  }

  Window::~Window() {
    glfwDestroyWindow(m_win);
  }

  void Window::clear() {
    glClearColor(m_col_bg.r, m_col_bg.g, m_col_bg.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void Window::processKeyboardInput() {
    if (glfwGetKey(m_win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(m_win, true);
    }
  }

} // namespace api::window
