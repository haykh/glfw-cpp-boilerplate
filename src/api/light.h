#ifndef API_LIGHT_H
#define API_LIGHT_H

#include "global.h"

#include "api/shader.h"

#include <string>

namespace api::light {
  using namespace api::shader;

  enum class LightType {
    Distant,
    Point,
    Spotlight
  };

  auto to_string(LightType type) -> std::string;

  static unsigned int LightId { 0 };

  class LightSource {
  protected:
    const unsigned int m_id;
    const LightType    m_type;

    color_t m_ambient_color { 1.0f };
    color_t m_diffuse_color { 1.0f };
    color_t m_specular_color { 1.0f };

    float m_ambient_strength { 1.0f };
    float m_diffuse_strength { 1.0f };
    float m_specular_strength { 1.0f };

  public:
    LightSource(LightType type) : m_id { LightId++ }, m_type { type } {}

    virtual void illuminate(const ShaderProgram&) const;
    void         print() const;

    // setters
    void setAmbientColor(const color_t& color) {
      m_ambient_color = color;
    }

    void setDiffuseColor(const color_t& color) {
      m_diffuse_color = color;
    }

    void setSpecularColor(const color_t& color) {
      m_specular_color = color;
    }

    void setColors(const color_t& ambient,
                   const color_t& diffuse,
                   const color_t& specular) {
      m_ambient_color  = ambient;
      m_diffuse_color  = diffuse;
      m_specular_color = specular;
    }

    void setAmbientStrength(float strength) {
      m_ambient_strength = strength;
    }

    void setDiffuseStrength(float strength) {
      m_diffuse_strength = strength;
    }

    void setSpecularStrength(float strength) {
      m_specular_strength = strength;
    }

    void setStrengths(float ambient, float diffuse, float specular) {
      m_ambient_strength  = ambient;
      m_diffuse_strength  = diffuse;
      m_specular_strength = specular;
    }

    // accessors
    [[nodiscard]]
    auto id() const -> unsigned int {
      return m_id;
    }

    [[nodiscard]]
    auto type() const -> LightType {
      return m_type;
    }

    [[nodiscard]]
    auto label() const -> std::string {
      return to_string(m_type) + "Light" + std::to_string(id());
    }

    [[nodiscard]]
    auto uniformLabel(const std::string& uniform) const -> std::string {
      return label() + "." + uniform;
    }

    [[nodiscard]]
    auto ambientColor() const -> color_t {
      return m_ambient_color;
    }

    [[nodiscard]]
    auto diffuseColor() const -> color_t {
      return m_diffuse_color;
    }

    [[nodiscard]]
    auto specularColor() const -> color_t {
      return m_specular_color;
    }

    [[nodiscard]]
    auto ambientStrength() const -> float {
      return m_ambient_strength;
    }

    [[nodiscard]]
    auto diffuseStrength() const -> float {
      return m_diffuse_strength;
    }

    [[nodiscard]]
    auto specularStrength() const -> float {
      return m_specular_strength;
    }

    [[nodiscard]]
    auto shaderDeclaration() const -> std::string;

    [[nodiscard]]
    auto shaderCall() const -> std::string;
  };

  class Positional : virtual public LightSource {
  protected:
    pos_t m_position { 0.0f };
    float m_constant { 1.0f };
    float m_linear { 0.09f };
    float m_quadratic { 0.032f };

  public:
    Positional(LightType type) : LightSource { type } {}

    virtual void illuminate(const ShaderProgram&) const override;

    void setPosition(const pos_t& pos) {
      m_position = pos;
    }

    void setConstant(float constant) {
      m_constant = constant;
    }

    void setLinear(float linear) {
      m_linear = linear;
    }

    void setQuadratic(float quadratic) {
      m_quadratic = quadratic;
    }

    void setAttenuation(float constant, float linear, float quadratic) {
      m_constant  = constant;
      m_linear    = linear;
      m_quadratic = quadratic;
    }

    [[nodiscard]]
    auto position() const -> pos_t {
      return m_position;
    }

    [[nodiscard]]
    auto constant() const -> float {
      return m_constant;
    }

    [[nodiscard]]
    auto linear() const -> float {
      return m_linear;
    }

    [[nodiscard]]
    auto quadratic() const -> float {
      return m_quadratic;
    }
  };

  class Directional : virtual public LightSource {
    vec_t m_direction { 0.0f, -1.0f, 0.0f };

  public:
    Directional(LightType type) : LightSource { type } {}

    virtual void illuminate(const ShaderProgram&) const override;

    void setDirection(const vec_t& dir) {
      m_direction = dir;
    }

    [[nodiscard]]
    auto direction() const -> vec_t {
      return m_direction;
    }
  };

  class Distant : public Directional {
  public:
    Distant()
      : LightSource { LightType::Distant }
      , Directional { LightType::Distant } {}
  };

  class Point : public Positional {
  public:
    Point()
      : LightSource { LightType::Point }
      , Positional { LightType::Point } {}
  };

  class Spotlight : public Directional,
                    public Positional {
    float m_cutoff { 12.5f };
    float m_outer_cutoff { 15.0f };

  public:
    Spotlight()
      : LightSource { LightType::Spotlight }
      , Directional { LightType::Spotlight }
      , Positional { LightType::Spotlight } {}

    void illuminate(const ShaderProgram&) const override;

    void setCutoffs(float cutoff, float outer_cutoff) {
      m_cutoff       = cutoff;
      m_outer_cutoff = outer_cutoff;
    }

    void setCutoff(float cutoff) {
      m_cutoff = cutoff;
    }

    void setOuterCutoff(float outer_cutoff) {
      m_outer_cutoff = outer_cutoff;
    }

    [[nodiscard]]
    auto cutoff() const -> float {
      return m_cutoff;
    }

    [[nodiscard]]
    auto outerCutoff() const -> float {
      return m_outer_cutoff;
    }
  };

} // namespace api::light

#endif // API_LIGHT_H
