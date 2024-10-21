#ifndef UTILS_ERROR_H
#define UTILS_ERROR_H

#include <string>
#define FILENAME(file)                                                         \
  (std::string(file).substr(std::string(file).find_last_of("/\\") + 1).c_str())

namespace raise {

  void error(const char*,
             const char* = FILENAME(__builtin_FILE()),
             int         = __builtin_LINE(),
             const char* = __builtin_FUNCTION());

  void error(const std::string&,
             const char* = FILENAME(__builtin_FILE()),
             int         = __builtin_LINE(),
             const char* = __builtin_FUNCTION());

} // namespace raise

#undef FILENAME
#endif // UTILS_ERROR_H
