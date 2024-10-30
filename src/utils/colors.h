#ifndef UTILS_COLORS_H
#define UTILS_COLORS_H

#include "global.h"

#include "utils/error.h"

#include <cstring>

namespace utils::color {
  using namespace utils;

  enum Colorspace {
    RGB,
    RGB_pcent,
    HSL,
    HSV,
    HEX
  };

  struct Convert {
    template <Colorspace C,
              typename std::enable_if<C == RGB || C == RGB_pcent, int>::type = 0>
    static auto from(char r, char g, char b) -> color_t {
      if constexpr (C == RGB) {
        return { r / 255.0f, g / 255.0f, b / 255.0f };
      } else if constexpr (C == RGB_pcent) {
        return { r / 100.0f, g / 100.0f, b / 100.0f };
      }
    }

    template <Colorspace C, typename std::enable_if<C == HEX, int>::type = 0>
    static auto from(const char* hex) -> color_t {
      if (hex[0] == '#') {
        hex++;
      }
      char r, g, b;
      if (strlen(hex) == 3) {
        r = (hex[0] - '0') * 16 + (hex[0] - '0');
        g = (hex[1] - '0') * 16 + (hex[1] - '0');
        b = (hex[2] - '0') * 16 + (hex[2] - '0');
      } else if (strlen(hex) == 6) {
        r = (hex[0] - '0') * 16 + (hex[1] - '0');
        g = (hex[2] - '0') * 16 + (hex[3] - '0');
        b = (hex[4] - '0') * 16 + (hex[5] - '0');
      }
      return Convert::from<RGB>(r, g, b);
    }

    template <Colorspace C, typename std::enable_if<C == HSL, int>::type = 0>
    static auto from(float h, float s, float l) -> color_t {
      if (h < 0 || h > 360 || s < 0 || s > 1 || l < 0 || l > 1) {
        raise::error("Invalid HSL values");
      }
      if (s <= 1e-4f) {
        return { l, l, l };
      } else {
        const auto hue2rgb = [](float p, float q, float t) -> float {
          if (t < 0.0f) {
            t += 1.0f;
          }
          if (t > 1.0f) {
            t -= 1.0f;
          }
          if (t < 1.0f / 6.0f) {
            return p + (q - p) * 6.0f * t;
          } else if (t < 0.5f) {
            return q;
          } else if (t < 2.0f / 3.0f) {
            return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
          } else {
            return p;
          }
        };
        const auto q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
        const auto p = 2.0f * l - q;
        return { hue2rgb(p, q, h + 1.0f / 3.0f),
                 hue2rgb(p, q, h),
                 hue2rgb(p, q, h - 1.0f / 3.0f) };
      }
    }
  };

} // namespace utils::color

#endif // UTILS_COLORS_H
