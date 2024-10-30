#ifndef GLOBAL_H
#define GLOBAL_H

#include <glm/glm.hpp>

#include <any>
#include <map>
#include <string>

using color_t = glm::vec3;
using vec_t   = glm::vec3;
using pos_t   = glm::vec3;

using transform_t = glm::mat4;

using config_t = std::map<std::string, std::any>;

#endif // GLOBAL_H
