#include "error.h"

#include "log.h"

#include <stdexcept>

namespace raise {

  void error(const char* msg, const char* file, int line, const char* func) {
    log::log(log::ERROR, msg, file, line, func);
    throw std::runtime_error(msg);
  }

} // namespace raise
