#ifndef API_WINDOW_H
#define API_WINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>

namespace api::window {

  class Window {
  private:
    GLFWwindow* m_win;
    int         m_winWidth, m_winHeight;
    float       m_col_bg[4];

  public:
    Window(int                width,
           int                height,
           const std::string& name,
           float              col_bg[4],
           bool               isResizable = true);
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
