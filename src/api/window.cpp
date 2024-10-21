#include "window.h"

#include "utils/error.h"
#include "utils/log.h"

#include <glad/gl.h>

#include <GLFW/glfw3.h>

namespace api {
  namespace log   = utils;
  namespace raise = utils;

  void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  }

  Window::Window(int                width,
                 int                height,
                 const std::string& name,
                 int                swapInterval,
                 float              col_bg[4],
                 bool               isResizable)
    : m_winWidth(width)
    , m_winHeight(height)
    , m_col_bg { col_bg[0], col_bg[1], col_bg[2], col_bg[3] } {
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
    glfwSetFramebufferSizeCallback(m_win, framebuffer_size_callback);
    gladLoadGL(glfwGetProcAddress);
  }

  Window::~Window() {
    glfwDestroyWindow(m_win);
  }

  void Window::processInput() {
    if (glfwGetKey(m_win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(m_win, true);
    }
  }

  void Window::clear() {
    glClearColor(m_col_bg[0], m_col_bg[1], m_col_bg[2], m_col_bg[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void Window::unuse() {
    glfwSwapBuffers(m_win);
    glfwPollEvents();
  }

} // namespace api
