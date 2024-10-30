#ifndef API_ENGINE_H
#define API_ENGINE_H

#include "global.h"

#include "utils/colors.h"

namespace engine {
  using namespace utils;

  void RenderLoop(float   = 1.0f,
                  int     = 2560,
                  int     = 1440,
                  color_t = color::Convert::from<color::HEX>("#383838"),
                  bool    = true);

} // namespace engine

#endif // API_ENGINE_H
