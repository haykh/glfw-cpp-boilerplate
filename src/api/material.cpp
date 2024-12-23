#include "material.h"

#include "api/shader.h"
#include "utils/error.h"

#include <any>
#include <cstdio>
#include <filesystem>
#include <string>

namespace api::material {
  using namespace utils;
  using namespace api::shader;

  auto to_string(MaterialType type) -> std::string {
    switch (type) {
      case MaterialType::Default:
        return "default";
      case MaterialType::Emitter:
        return "emitter";
      default:
        raise::error("unknown material type");
        return "";
    }
  }

  auto to_inShaderId(MaterialType type) -> unsigned int {
    switch (type) {
      case MaterialType::Default:
        return 1;
      case MaterialType::Emitter:
        return 2;
      default:
        raise::error("unknown material type");
        return 0;
    }
  }

  auto from_inShaderId(unsigned int id) -> std::string {
    switch (id) {
      case 1:
        return to_string(MaterialType::Default);
      case 2:
        return to_string(MaterialType::Emitter);
      default:
        raise::error("unknown material id");
        return "";
    }
  }

  auto shaderDeclaration(unsigned int id, unsigned int N) -> std::string {
    auto shader_type = from_inShaderId(id);
    shader_type[0]   = std::toupper(shader_type[0]);
    return "uniform " + shader_type + "Material " + from_inShaderId(id) +
           "Material[" + std::to_string(N) + "];";
  }

  void Material::shade(const ShaderProgram& shader) const {
    shader.setUniform1ui("activeMesh.matId", inShaderId());
    shader.setUniform1ui("activeMesh.matIdx", id());
  }

  void Material::print() const {
    printf("%s", label().c_str());
  }

  void Normal::set(const std::string& key, std::any value) {
    if (key == "shininess") {
      m_shininess = std::any_cast<float>(value);
    } else if (key == "diffuseTexture") {
      m_diffuseTexture = new Texture(
        std::any_cast<std::filesystem::path>(value).generic_string());
    } else if (key == "specularTexture") {
      m_specularTexture = new Texture(
        std::any_cast<std::filesystem::path>(value).generic_string());
    } else {
      raise::error("unknown key");
    }
  }

  void Normal::shade(const ShaderProgram& shader) const {
    Material::shade(shader);
    if (diffuseTexture() != nullptr) {
      shader.setUniform1i(uniformLabel("diffuseMap"), 0);
      diffuseTexture()->use(0);
    }
    if (specularTexture() != nullptr) {
      shader.setUniform1i(uniformLabel("specularMap"), 1);
      specularTexture()->use(1);
    }
    shader.setUniform1f(uniformLabel("shininess"), shininess());
  }

  void Emitter::set(const std::string& key, std::any value) {
    if (key == "color") {
      m_color = std::any_cast<color_t>(value);
    } else {
      raise::error("unknown key");
    }
  }

  void Emitter::shade(const ShaderProgram& shader) const {
    Material::shade(shader);
    shader.setUniform3fv(uniformLabel("color"), color());
  }

} // namespace api::material
