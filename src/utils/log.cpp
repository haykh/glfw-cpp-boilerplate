#include "log.h"

#include "colors.h"

#include <cstdio>
#include <stdexcept>
#include <string>

namespace utils {

  auto logLevel2Color(LogLevel level) -> const char* {
    switch (level) {
      case DEBUG:
        return color::get(color::BLUE);
        break;
      case INFO:
        return color::get(color::BRIGHT_BLACK);
      case NORMAL:
        return color::get(color::WHITE);
      case SUCCESS:
        return color::get(color::GREEN);
      case WARNING:
        return color::get(color::BRIGHT_YELLOW);
      case ERROR:
        return color::get(color::BRIGHT_RED);
      default:
        throw std::runtime_error("Invalid log level");
    }
  }

  auto logLevel2Str(LogLevel level) -> const char* {
    switch (level) {
      case DEBUG:
        return "DEBUG";
      case INFO:
        return "INFO";
      case NORMAL:
        return "NORMAL";
      case SUCCESS:
        return "SUCCESS";
      case WARNING:
        return "WARNING";
      case ERROR:
        return "ERROR";
      default:
        throw std::runtime_error("Invalid log level");
    }
  }

  void log(LogLevel    level,
           const char* msg,
           const char* file,
           int         line,
           const char* func) noexcept {
    fprintf(level != ERROR ? stdout : stderr,
            "%s%-10s%-8s [%s@%d in %s]\n└> %s%s\n",
            logLevel2Color(level),
            __TIME__,
            logLevel2Str(level),
            file,
            line,
            func,
            msg,
            color::get(color::RESET));
  }

  void log(LogLevel           level,
           const std::string& msg,
           const char*        file,
           int                line,
           const char*        func) noexcept {
    log(level, msg.c_str(), file, line, func);
  }

} // namespace utils
