#ifndef API_WINDOW_H
#define API_WINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>

namespace api {

  class Window {
  private:
    GLFWwindow* m_win;
    float       m_col_bg[4];
    int         m_winWidth, m_winHeight;

  public:
    Window(int                width,
           int                height,
           const std::string& name,
           int                swapInterval,
           float              col_bg[4],
           bool               isResizable = true);
    ~Window();
    void processInput();
    void unuse();
    void clear();

    // accessors
    [[nodiscard]]
    auto windowShouldClose() const noexcept -> int {
      return glfwWindowShouldClose(m_win);
    }

    [[nodiscard]]
    auto window() const noexcept -> GLFWwindow* {
      return m_win;
    }

    [[nodiscard]]
    auto aspect() const noexcept -> float {
      return (float)m_winWidth / (float)m_winHeight;
    }
  };

} // namespace api

#endif // API_WINDOW_H
