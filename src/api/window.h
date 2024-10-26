#ifndef API_WINDOW_H
#define API_WINDOW_H

#include "global.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>

namespace api::window {

  class Window {
  private:
    GLFWwindow* m_win;
    int         m_winWidth, m_winHeight;
    color_t     m_col_bg;

  public:
    Window(int, int, const std::string&, const color_t&, bool = true);
    ~Window();

    void processKeyboardInput();
    void clear();

    // accessors
    [[nodiscard]]
    auto windowShouldClose() const -> int {
      return glfwWindowShouldClose(m_win);
    }

    [[nodiscard]]
    auto window() const -> GLFWwindow* {
      return m_win;
    }

    [[nodiscard]]
    auto aspect() const -> float {
      return (float)m_winWidth / (float)m_winHeight;
    }
  };

} // namespace api::window

#endif // API_WINDOW_H
