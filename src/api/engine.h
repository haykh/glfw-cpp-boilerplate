#ifndef API_ENGINE_H
#define API_ENGINE_H

#include "api/window.h"
#include <memory>

namespace engine {

  class Engine {
    std::unique_ptr<api::Window> m_window;

  public:
    Engine(float scale      = 1.0f,
           int   win_width  = 2560,
           int   win_height = 1440,
           float col_bg[4]  = new float[] { 0.2f, 0.3f, 0.3f, 1.0f },
           bool  resizable  = true);

    ~Engine() = default;

    void renderLoop();
  };

} // namespace engine

#endif // API_ENGINE_H
