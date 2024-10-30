#ifndef API_MATERIAL_H
#define API_MATERIAL_H

#include "global.h"

#include "api/object.h"
#include "api/shader.h"
#include "api/texture.h"

#include <string>

namespace api::material {
  using namespace api::shader;
  using namespace api::texture;
  using namespace api::object;

  enum class MaterialType {
    Default,
    Emitter
  };

  auto to_string(MaterialType) -> std::string;
  auto to_inShaderId(MaterialType) -> unsigned int;

  auto from_inShaderId(unsigned int) -> std::string;

  auto shaderDeclaration(unsigned int, unsigned int) -> std::string;

  class Material : public Object {
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
    float    m_shininess { 64.0f };
    Texture* m_diffuseTexture { nullptr };
    Texture* m_specularTexture { nullptr };

  public:
    Normal(unsigned int id, MaterialType type, const std::string& name)
      : Material(id, type, name) {}

    ~Normal() {
      if (m_diffuseTexture != nullptr) {
        delete m_diffuseTexture;
      }
      if (m_specularTexture != nullptr) {
        delete m_specularTexture;
      }
    }

    virtual void shade(const ShaderProgram&) const override;
    virtual void set(const std::string& key, std::any value) override;

    // accessors
    [[nodiscard]]
    auto shininess() const -> float {
      return m_shininess;
    }

    [[nodiscard]]
    auto diffuseTexture() const -> Texture* {
      return m_diffuseTexture;
    }

    [[nodiscard]]
    auto specularTexture() const -> Texture* {
      return m_specularTexture;
    }
  };

  static unsigned int DefaultMaterialId { 0 };
  static unsigned int EmitterMaterialId { 0 };

  class Default : public Normal {
  public:
    Default(const std::string& name, const config_t& params = {})
      : Normal(DefaultMaterialId++, MaterialType::Default, name) {
      configure(params);
    }
  };

  class Emitter : public Material {
    color_t m_color;

  public:
    Emitter(const std::string& name, const config_t& params = {})
      : Material(EmitterMaterialId++, MaterialType::Emitter, name) {
      configure(params);
    }

    void shade(const ShaderProgram&) const override;
    void set(const std::string&, std::any) override;

    // accessors
    [[nodiscard]]
    auto color() const -> color_t {
      return m_color;
    }
  };

} // namespace api::material

#endif // API_MATERIAL_H
