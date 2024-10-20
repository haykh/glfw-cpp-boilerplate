#ifndef API_ENGINE_H
#define API_ENGINE_H

#include "api/window.h"
#include <memory>

namespace engine {

  class Engine {
    std::unique_ptr<Window> m_window;

  public:
    Engine(float scale      = 1.0f,
           int   win_width  = 2560,
           int   win_height = 1440,
           bool  resizable  = true);

    ~Engine() = default;

    void renderLoop();
  };

} // namespace engine

#endif // API_ENGINE_H
