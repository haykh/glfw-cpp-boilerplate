#include "error.h"

#include "log.h"

#include <stdexcept>
#include <string>

namespace utils::raise {

  void error(const char* msg, const char* file, int line, const char* func) {
    utils::log::log(utils::log::ERROR, msg, file, line, func);
    throw std::runtime_error(msg);
  }

  void error(const std::string& msg,
             const char*        file,
             int                line,
             const char*        func) {
    error(msg.c_str(), file, line, func);
  }

} // namespace utils::raise
