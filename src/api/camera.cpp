#include "camera.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace api {

  void Camera::processInput(GLFWwindow* window, float dt) {
    const auto norm_speed = speed() * dt;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      setPosition(position() + norm_speed * front());
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      setPosition(position() - norm_speed * front());
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      setPosition(
        position() - glm::normalize(glm::cross(front(), up())) * norm_speed);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      setPosition(
        position() + glm::normalize(glm::cross(front(), up())) * norm_speed);
    }
  }

  void Camera::pointAt(const glm::vec3& target) {
    const auto front = glm::normalize(target - m_position);
    m_yaw            = glm::degrees(atan2(front.z, front.x));
    m_pitch          = glm::degrees(asin(front.y));
  }

  auto Camera::view() const -> glm::mat4 {
    const auto frDir = front();
    const auto upDir = up(frDir);
    return glm::lookAt(m_position, m_position + frDir, upDir);
  }

  auto Camera::project() const -> glm::mat4 {
    if (m_type == CameraType::ORTHOGRAPHIC) {
      return glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, zNear(), zFar());
    } else {
      return glm::perspective(glm::radians(fov()), aspect(), zNear(), zFar());
    }
  }

  // const glm::vec3& target

} // namespace api
