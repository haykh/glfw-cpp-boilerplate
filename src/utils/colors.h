#ifndef UTILS_COLORS_H
#define UTILS_COLORS_H

#include <string>

namespace utils::color {

  enum Color {
    RESET,
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    BRIGHT_BLACK,
    BRIGHT_RED,
    BRIGHT_GREEN,
    BRIGHT_YELLOW,
    BRIGHT_BLUE,
    BRIGHT_MAGENTA,
    BRIGHT_CYAN,
    BRIGHT_WHITE
  };

  auto strip(const std::string& msg) noexcept -> std::string;

  auto get(Color) noexcept -> const char*;

} // namespace utils::color

#endif // UTILS_COLORS_H
