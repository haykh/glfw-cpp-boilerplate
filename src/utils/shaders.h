#ifndef UTILS_SHADERS_H
#define UTILS_SHADERS_H

#include <map>
#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace shaders {

  [[nodiscard]]
  auto createShaderProgram(const std::string&, const char*, const char*)
    -> unsigned int;

  [[nodiscard]]
  auto createVertexBuffers(const std::string&,
                           const float*,
                           std::size_t,
                           const unsigned int*,
                           std::size_t) -> std::map<std::string, unsigned int>;

  void checkCompilation(unsigned int id, const std::string& shader);
  void checkLinking(unsigned int id, const std::string& program);

} // namespace shaders

#endif // UTILS_SHADERS_H
