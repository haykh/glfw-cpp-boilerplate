#ifndef API_LIGHT_H
#define API_LIGHT_H

#include "api/camera.h"
#include "api/mesh.h"
#include "api/shader.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace api {

  class Light {
    unsigned int m_vao;

    float     m_intensity = 1.0f;
    glm::vec3 m_color;
    glm::vec3 m_position;

  public:
    ShaderProgram shader;

    Light(const std::string&,
          const std::string&,
          float            = 1.0f,
          const glm::vec3& = glm::vec3(1.0f),
          const glm::vec3& = glm::vec3(0.0f));

    void bind(unsigned int) const;
    void render(const std::vector<const Mesh*>&, const Camera&, float) const;

    // setters
    void setIntensity(float intensity) {
      m_intensity = intensity;
    }

    void setColor(const glm::vec3& col) {
      m_color = col;
    }

    void setPosition(const glm::vec3& pos) {
      m_position = pos;
    }

    // accessors
    [[nodiscard]]
    auto intensity() const -> float {
      return m_intensity;
    }

    [[nodiscard]]
    auto color() const -> glm::vec3 {
      return m_color;
    }

    [[nodiscard]]
    auto position() const -> glm::vec3 {
      return m_position;
    }
  };

} // namespace api

#endif // API_LIGHT_H
