#ifndef UTILS_TICKER_H
#define UTILS_TICKER_H

#include <glad/gl.h>

#include <GLFW/glfw3.h>

namespace utils::timer {

  class Ticker {
    float m_lastTime { 0.0f };
    float m_deltaTime { 0.0f };
    float m_currentTime { 0.0f };

  public:
    void tick() {
      m_currentTime = (float)glfwGetTime();
      m_deltaTime   = m_currentTime - m_lastTime;
      m_lastTime    = m_currentTime;
    }

    [[nodiscard]]
    auto time() const -> float {
      return m_currentTime;
    }

    [[nodiscard]]
    auto dt() const -> float {
      return m_deltaTime;
    }
  };

} // namespace utils::timer

#endif // UTILS_TICKER_H
