#ifndef API_WINDOW_H
#define API_WINDOW_H

#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

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

    [[nodiscard]]
    auto windowShouldClose() const -> int {
      return glfwWindowShouldClose(m_win);
    }

    [[nodiscard]]
    auto get_window() const -> GLFWwindow* {
      return m_win;
    }
  };

} // namespace api

#endif // API_WINDOW_H
