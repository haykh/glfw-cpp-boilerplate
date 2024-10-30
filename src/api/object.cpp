#include "object.h"

namespace api::object {

  void Object::configure(const config_t& config) {
    for (const auto& [key, value] : config) {
      set(key, value);
    }
  }

} // namespace api::object
