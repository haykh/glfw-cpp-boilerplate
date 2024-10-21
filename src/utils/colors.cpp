#include "colors.h"

#include <cstring>
#include <string>

namespace color {

  auto strip(const std::string& msg) noexcept -> std::string {
    static constexpr const char* all[] = {
      "\033[0m",  "\033[30m", "\033[31m", "\033[32m", "\033[33m", "\033[34m",
      "\033[35m", "\033[36m", "\033[37m", "\033[90m", "\033[91m", "\033[92m",
      "\033[93m", "\033[94m", "\033[95m", "\033[96m", "\033[97m"
    };
    auto msg_nocol = msg;
    for (const auto c : all) {
      std::size_t pos = 0;
      while ((pos = msg_nocol.find(c, pos)) != std::string::npos) {
        msg_nocol.replace(pos, strlen(c), "");
        pos += strlen("");
      }
    }
    return msg_nocol;
  }

  auto get(Color c) noexcept -> const char* {
    switch (c) {
      case RESET:
        return "\033[0m";
      case BLACK:
        return "\033[30m";
      case RED:
        return "\033[31m";
      case GREEN:
        return "\033[32m";
      case YELLOW:
        return "\033[33m";
      case BLUE:
        return "\033[34m";
      case MAGENTA:
        return "\033[35m";
      case CYAN:
        return "\033[36m";
      case WHITE:
        return "\033[37m";
      case BRIGHT_BLACK:
        return "\033[90m";
      case BRIGHT_RED:
        return "\033[91m";
      case BRIGHT_GREEN:
        return "\033[92m";
      case BRIGHT_YELLOW:
        return "\033[93m";
      case BRIGHT_BLUE:
        return "\033[94m";
      case BRIGHT_MAGENTA:
        return "\033[95m";
      case BRIGHT_CYAN:
        return "\033[96m";
      case BRIGHT_WHITE:
        return "\033[97m";
      default:
        return "";
    }
  }

} // namespace color
