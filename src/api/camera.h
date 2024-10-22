#ifndef API_CAMERA_H
#define API_CAMERA_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace api {

  enum class CameraType {
    PERSPECTIVE,
    ORTHOGRAPHIC
  };

  class Camera {
    CameraType m_type { CameraType::PERSPECTIVE };

    glm::vec3 m_position { 0.0f, 0.0f, 0.0f };

    float m_fov { 45.0f };
    float m_aspect { 1.0f };
    float m_zNear { 0.1f };
    float m_zFar { 100.0f };

    // primary parameters
    float m_yaw { -90.0f };
    float m_pitch { 0.0f };
    float m_roll { 0.0f };

  public:
    static constexpr glm::vec3 WorldUp { 0.0f, 1.0f, 0.0f };
    static constexpr float     Speed { 5.0f };
    static constexpr float     Sensitivity { 0.1f };

    Camera() = default;

    void processInput(GLFWwindow*, float);

    void recalcOrientation();

    // setters
    void setType(const CameraType& type) {
      m_type = type;
    }

    void setPosition(const glm::vec3& position) {
      m_position = position;
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

    void setYaw(float yaw) {
      m_yaw = yaw;
    }

    void setPitch(float pitch) {
      m_pitch = pitch;
    }

    void setRoll(float roll) {
      m_roll = roll;
    }

    // toggles
    void toggleType() {
      setType((m_type == CameraType::PERSPECTIVE) ? CameraType::ORTHOGRAPHIC
                                                  : CameraType::PERSPECTIVE);
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
    auto speed() const -> float {
      return Speed;
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
    auto yaw() const -> float {
      return m_yaw;
    }

    [[nodiscard]]
    auto pitch() const -> float {
      return m_pitch;
    }

    [[nodiscard]]
    auto roll() const -> float {
      return m_roll;
    }

    [[nodiscard]]
    auto right(const glm::vec3& front) const -> glm::vec3 {
      return glm::normalize(glm::cross(front, WorldUp));
    }

    [[nodiscard]]
    auto right() const -> glm::vec3 {
      return right(front());
    }

    [[nodiscard]]
    auto left(const glm::vec3& front) const -> glm::vec3 {
      return -right(front);
    }

    [[nodiscard]]
    auto left() const -> glm::vec3 {
      return left(front());
    }

    [[nodiscard]]
    auto up(const glm::vec3& front, const glm::vec3& right) const -> glm::vec3 {
      return glm::normalize(glm::cross(right, front));
    }

    [[nodiscard]]
    auto up(const glm::vec3& front) const -> glm::vec3 {
      return glm::normalize(glm::cross(right(front), front));
    }

    [[nodiscard]]
    auto up() const -> glm::vec3 {
      return glm::normalize(glm::cross(right(), front()));
    }

    [[nodiscard]]
    auto front() const -> glm::vec3 {
      return glm::vec3(cos(glm::radians(yaw())) * cos(glm::radians(pitch())),
                       sin(glm::radians(pitch())),
                       sin(glm::radians(yaw())) * cos(glm::radians(pitch())));
    }

    // methods
    void pointAt(const glm::vec3&);

    [[nodiscard]]
    auto view() const -> glm::mat4;

    [[nodiscard]]
    auto project() const -> glm::mat4;
  };

} // namespace api

#endif // API_CAMERA_H
