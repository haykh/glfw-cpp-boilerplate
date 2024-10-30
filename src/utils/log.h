#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <string>

#define FILENAME(file)                                                         \
  (std::string(file).substr(std::string(file).find_last_of("/\\") + 1).c_str())

namespace utils::log {

  namespace color {
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

    auto strip(const std::string& msg) -> std::string;

    auto get(Color) -> const char*;
  } // namespace color

  enum LogLevel {
    DEBUG,
    INFO,
    NORMAL,
    SUCCESS,
    WARNING,
    ERROR,
  };

  void log(LogLevel,
           const char*,
           const char* = FILENAME(__builtin_FILE()),
           int         = __builtin_LINE(),
           const char* = __builtin_FUNCTION());

  void log(LogLevel,
           const std::string&,
           const char* = FILENAME(__builtin_FILE()),
           int         = __builtin_LINE(),
           const char* = __builtin_FUNCTION());

} // namespace utils::log

#undef FILENAME
#endif // UTILS_LOG_H
