#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <string>

#define FILENAME(file)                                                         \
  (std::string(file).substr(std::string(file).find_last_of("/\\") + 1).c_str())

namespace utils {

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
           const char* = __builtin_FUNCTION()) noexcept;

  void log(LogLevel,
           const std::string&,
           const char* = FILENAME(__builtin_FILE()),
           int         = __builtin_LINE(),
           const char* = __builtin_FUNCTION()) noexcept;

} // namespace utils

#undef FILENAME
#endif // UTILS_LOG_H
