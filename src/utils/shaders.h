#ifndef UTILS_SHADERS_H
#define UTILS_SHADERS_H

#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace shaders {

  [[nodiscard]]
  auto createShaderProgram(const std::string&, const char*, const char*)
    -> unsigned int;
  [[nodiscard]]
  auto createVAO(const std::string&, const float*, std::size_t) -> unsigned int;

  void checkCompilation(unsigned int id, const std::string& shader);
  void checkLinking(unsigned int id, const std::string& program);

} // namespace shaders

#endif // UTILS_SHADERS_H
