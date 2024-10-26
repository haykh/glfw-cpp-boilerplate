#ifndef API_ENGINE_H
#define API_ENGINE_H

#include "global.h"

namespace engine {

  void RenderLoop(float   = 1.0f,
                  int     = 2560,
                  int     = 1440,
                  color_t = { 0.0f, 0.0f, 0.0f },
                  bool    = true);

} // namespace engine

#endif // API_ENGINE_H
