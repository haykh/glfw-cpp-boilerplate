#ifndef API_MATERIAL_H
#define API_MATERIAL_H

#include "global.h"

#include "api/shader.h"

#include <string>

namespace api::material {
  using namespace api::shader;

  enum class MaterialType {
    Default,
    Emitter
  };

  auto to_string(MaterialType) -> std::string;
  auto to_inShaderId(MaterialType) -> unsigned int;

  auto from_inShaderId(unsigned int) -> std::string;

  auto shaderDeclaration(unsigned int, unsigned int) -> std::string;

  class Material {
  protected:
    const unsigned int m_id;
    const MaterialType m_type;
    const std::string  m_name;

  public:
    Material(unsigned int id, MaterialType type, const std::string& name)
      : m_id { id }
      , m_type { type }
      , m_name { name } {}

    virtual void shade(const ShaderProgram&) const;
    void         print() const;

    // accessors
    [[nodiscard]]
    auto id() const -> unsigned int {
      return m_id;
    }

    [[nodiscard]]
    auto type() const -> MaterialType {
      return m_type;
    }

    [[nodiscard]]
    auto inShaderId() const -> unsigned int {
      return to_inShaderId(type());
    }

    [[nodiscard]]
    auto name() const -> std::string {
      return m_name;
    }

    [[nodiscard]]
    auto label() const -> std::string {
      return to_string(type()) + "Material[" + std::to_string(id()) + "]";
    }

    [[nodiscard]]
    auto uniformLabel(const std::string& uniform) const -> std::string {
      return to_string(type()) + "Material[" + std::to_string(id()) + "]." +
             uniform;
    }
  };

  class Normal : public Material {
    color_t m_diffuseColor { 0.7f, 0.7f, 0.7f };
    color_t m_specularColor { 1.0f, 1.0f, 1.0f };
    float   m_shininess { 64.0f };

  public:
    Normal(unsigned int id, MaterialType type, const std::string& name)
      : Material(id, type, name) {}

    virtual void shade(const ShaderProgram&) const override;

    // setters
    void setDiffuseColor(const color_t& color) {
      m_diffuseColor = color;
    }

    void setSpecularColor(const color_t& color) {
      m_specularColor = color;
    }

    void setShininess(float shininess) {
      m_shininess = shininess;
    }

    // accessors
    [[nodiscard]]
    auto diffuseColor() const -> color_t {
      return m_diffuseColor;
    }

    [[nodiscard]]
    auto specularColor() const -> color_t {
      return m_specularColor;
    }

    [[nodiscard]]
    auto shininess() const -> float {
      return m_shininess;
    }
  };

  static unsigned int DefaultMaterialId { 0 };
  static unsigned int EmitterMaterialId { 0 };

  class Default : public Normal {
  public:
    Default(const std::string& name)
      : Normal(DefaultMaterialId++, MaterialType::Default, name) {}
  };

  class Emitter : public Material {
    color_t m_color;

  public:
    Emitter(const std::string& name)
      : Material(EmitterMaterialId++, MaterialType::Emitter, name) {}

    void shade(const ShaderProgram&) const override;

    // setters
    void setColor(const color_t& color) {
      m_color = color;
    }

    // accessors
    [[nodiscard]]
    auto color() const -> color_t {
      return m_color;
    }
  };

} // namespace api::material

#endif // API_MATERIAL_H
