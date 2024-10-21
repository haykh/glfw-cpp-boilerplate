#ifndef API_CAMERA_H
#define API_CAMERA_H

#include "utils/error.h"

#include <glm/glm.hpp>

namespace api {
  namespace raise = utils;

  enum class CameraType {
    PERSPECTIVE,
    ORTHOGRAPHIC
  };

  class Camera {
    CameraType m_type;

    glm::vec3 m_position;
    glm::vec3 m_up;
    glm::vec3 m_target;
    glm::vec3 m_front;

    float m_fov;
    float m_aspect;
    float m_zNear;
    float m_zFar;

    bool m_stare { false };

  public:
    Camera(const glm::vec3&  position = glm::vec3(0.0f, 0.0f, 0.0f),
           const glm::vec3&  up       = glm::vec3(0.0f, 1.0f, 0.0f),
           const glm::vec3&  target   = glm::vec3(0.0f, 0.0f, 0.0f),
           const CameraType& type     = CameraType::PERSPECTIVE,
           float             fov      = 45.0f,
           float             aspect   = 1.0f,
           float             zNear    = 0.1f,
           float             zFar     = 100.0f)
      : m_type { type }
      , m_position { position }
      , m_up { up }
      , m_target { target }
      , m_front { target - position }
      , m_fov { fov }
      , m_aspect { aspect }
      , m_zNear { zNear }
      , m_zFar { zFar } {}

    // setters
    void setType(const CameraType& type) {
      m_type = type;
    }

    void setPosition(const glm::vec3& position) {
      m_position = position;
    }

    void setUp(const glm::vec3& up) {
      m_up = up;
    }

    void setFront(const glm::vec3& front) {
      if (stare()) {
        raise::error("cannot set front explicitly vector when staring");
      } else {
        m_front  = front;
        m_target = m_position + m_front;
      }
    }

    void setTarget(const glm::vec3& target) {
      m_target = target;
      m_front  = target - m_position;
    }

    void setFOV(float fov) {
      m_fov = fov;
    }

    void setAspect(float aspect) {
      m_aspect = aspect;
    }

    void setZNear(float zNear) {
      m_zNear = zNear;
    }

    void setZFar(float zFar) {
      m_zFar = zFar;
    }

    void setStare(float stare) {
      m_stare = stare;
    }

    // toggles
    void toggleType() {
      setType((m_type == CameraType::PERSPECTIVE) ? CameraType::ORTHOGRAPHIC
                                                  : CameraType::PERSPECTIVE);
    }

    void toggleStare() {
      setStare(!m_stare);
    }

    // accessors
    [[nodiscard]]
    auto type() const -> CameraType {
      return m_type;
    }

    [[nodiscard]]
    auto position() const -> const glm::vec3& {
      return m_position;
    }

    [[nodiscard]]
    auto up() const -> const glm::vec3& {
      return m_up;
    }

    [[nodiscard]]
    auto target() const -> const glm::vec3& {
      return m_target;
    }

    [[nodiscard]]
    auto front() const -> const glm::vec3& {
      return m_front;
    }

    [[nodiscard]]
    auto fov() const -> float {
      return m_fov;
    }

    [[nodiscard]]
    auto aspect() const -> float {
      return m_aspect;
    }

    [[nodiscard]]
    auto zNear() const -> float {
      return m_zNear;
    }

    [[nodiscard]]
    auto zFar() const -> float {
      return m_zFar;
    }

    [[nodiscard]]
    auto stare() const -> bool {
      return m_stare;
    }

    // methods
    [[nodiscard]]
    auto view() const -> glm::mat4;

    [[nodiscard]]
    auto project() const -> glm::mat4;
  };

} // namespace api

#endif // API_CAMERA_H
