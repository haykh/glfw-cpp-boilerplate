#include "light.h"

#include "api/shader.h"
#include "utils/error.h"

#include <cstdio>

namespace api::light {
  using namespace utils;
  using namespace api::shader;

  auto to_string(LightType type) -> std::string {
    switch (type) {
      case LightType::Distant:
        return "distant";
      case LightType::Point:
        return "point";
      case LightType::Spotlight:
        return "spotlight";
      default:
        raise::error("invalid light type");
        return "";
    }
  }

  void LightSource::print() const {
    if (m_id == 0) {
      printf("%s ", label().c_str());
    } else {
      printf("%s[%d] ", label().c_str(), m_id);
    }
  }

  void LightSource::illuminate(const ShaderProgram& shader) const {
    shader.setUniform3fv(uniformLabel("ambientColor"), ambientColor());
    shader.setUniform3fv(uniformLabel("diffuseColor"), diffuseColor());
    shader.setUniform3fv(uniformLabel("specularColor"), specularColor());
    shader.setUniform1f(uniformLabel("ambientStrength"), ambientStrength());
    shader.setUniform1f(uniformLabel("diffuseStrength"), diffuseStrength());
    shader.setUniform1f(uniformLabel("specularStrength"), specularStrength());
  }

  void Positional::illuminate(const ShaderProgram& shader) const {
    LightSource::illuminate(shader);
    shader.setUniform3fv(uniformLabel("position"), position());
  }

  void Directional::illuminate(const ShaderProgram& shader) const {
    LightSource::illuminate(shader);
    shader.setUniform3fv(uniformLabel("direction"), direction());
  }

  void Point::illuminate(const ShaderProgram& shader) const {
    Positional::illuminate(shader);
    shader.setUniform1f(uniformLabel("constant"), constant());
    shader.setUniform1f(uniformLabel("linear"), linear());
    shader.setUniform1f(uniformLabel("quadratic"), quadratic());
  }

  void Spotlight::illuminate(const ShaderProgram& shader) const {
    Directional::illuminate(shader);
    Positional::illuminate(shader);
    shader.setUniform1f(uniformLabel("cutoff"), cutoff());
    shader.setUniform1f(uniformLabel("outerCutoff"), outerCutoff());
  }

} // namespace api::light
