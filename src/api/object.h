#ifndef API_OBJECT_H
#define API_OBJECT_H

#include "global.h"

#include <string>

namespace api::object {

  struct Object {
    virtual void set(const std::string&, std::any) = 0;
    void         configure(const config_t&);
  };

} // namespace api::object

#endif // API_OBJECT_H
