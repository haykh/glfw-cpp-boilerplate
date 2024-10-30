#include "light.h"

#include "api/shader.h"
#include "utils/error.h"

#include <algorithm>
#include <cstdio>
#include <string>

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
    printf("%sLight%u\n", to_string(m_type).c_str(), id());
  }

  void LightSource::illuminate(const ShaderProgram& shader) const {
    shader.setUniform3fv(uniformLabel("ambientColor"), ambientColor());
    shader.setUniform3fv(uniformLabel("diffuseColor"), diffuseColor());
    shader.setUniform3fv(uniformLabel("specularColor"), specularColor());
    shader.setUniform1f(uniformLabel("ambientStrength"), ambientStrength());
    shader.setUniform1f(uniformLabel("diffuseStrength"), diffuseStrength());
    shader.setUniform1f(uniformLabel("specularStrength"), specularStrength());
  }

  [[nodiscard]]
  auto LightSource::shaderDeclaration() const -> std::string {
    auto type = to_string(m_type);
    std::transform(type.begin(), type.begin() + 1, type.begin(), ::toupper);
    return "uniform " + type + "Light " + label() + ";";
  }

  [[nodiscard]]
  auto LightSource::shaderCall() const -> std::string {
    if (m_type == LightType::Point) {
      return "result += CalcPointLight(" + label() +
             ", norm, FragPos, viewDir, defaultMaterial[activeMesh.matIdx]);";
    } else if (m_type == LightType::Distant) {
      return "result += CalcDistantLight(" + label() +
             ", norm, viewDir, defaultMaterial[activeMesh.matIdx]);";
    } else if (m_type == LightType::Spotlight) {
      return "result += CalcSpotLight(" + label() +
             ", norm, FragPos, viewDir, defaultMaterial[activeMesh.matIdx]);";
    } else {
      raise::error("light type not recognized");
      return "";
    }
  }

  void Positional::illuminate(const ShaderProgram& shader) const {
    LightSource::illuminate(shader);
    shader.setUniform3fv(uniformLabel("position"), position());
    shader.setUniform1f(uniformLabel("constant"), constant());
    shader.setUniform1f(uniformLabel("linear"), linear());
    shader.setUniform1f(uniformLabel("quadratic"), quadratic());
  }

  void Directional::illuminate(const ShaderProgram& shader) const {
    LightSource::illuminate(shader);
    shader.setUniform3fv(uniformLabel("direction"), direction());
  }

  void Spotlight::illuminate(const ShaderProgram& shader) const {
    Directional::illuminate(shader);
    Positional::illuminate(shader);
    shader.setUniform1f(uniformLabel("cutOff"),
                        glm::cos(glm::radians(cutoff())));
    shader.setUniform1f(uniformLabel("outerCutOff"),
                        glm::cos(glm::radians(outerCutoff())));
  }

} // namespace api::light
