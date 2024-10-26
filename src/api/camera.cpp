#include "camera.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>

namespace api::camera {

  void Camera::mouseInputCallback(GLFWwindow* win, double xPos, double yPos) {
    static bool  firstMouse = true;
    static float lastX      = 0.0f;
    static float lastY      = 0.0f;
    auto         cam = static_cast<Camera*>(glfwGetWindowUserPointer(win));
    if (firstMouse) {
      firstMouse = false;
      lastX      = xPos;
      lastY      = yPos;
    }
    float xOffset = (xPos - lastX) * cam->sensitivityX();
    float yOffset = (lastY - yPos) * cam->sensitivityY();
    cam->setYaw(cam->yaw() + xOffset);
    cam->setPitch(cam->pitch() + yOffset);
    lastX = xPos;
    lastY = yPos;
  }

  void Camera::processKeyboardInput(GLFWwindow* window, float dt) {
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
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      setPosition(position() + 0.5f * norm_speed * up());
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
      setPosition(position() - 0.5f * norm_speed * up());
    }
  }

  void Camera::pointAt(const pos_t& target) {
    const auto front = glm::normalize(target - m_position);
    m_yaw            = glm::degrees(atan2(front.z, front.x));
    m_pitch          = glm::degrees(asin(front.y));
  }

  auto Camera::view() const -> transform_t {
    const auto frDir = front();
    const auto upDir = up(frDir);
    return glm::lookAt(m_position, m_position + frDir, upDir);
  }

  auto Camera::project() const -> transform_t {
    if (m_type == CameraType::Orthographic) {
      return glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, zNear(), zFar());
    } else {
      return glm::perspective(glm::radians(fov()), aspect(), zNear(), zFar());
    }
  }

  void Camera::print() const {
    printf("%s : pos [%.2f %.2f %.2f] : fwd [%.2f %.2f %.2f] : fov %.2f : "
           "[%.2f - %.2f]",
           type() == CameraType::Perspective ? "Persp" : "Orth",
           position().x,
           position().y,
           position().z,
           front().x,
           front().y,
           front().z,
           fov(),
           zNear(),
           zFar());
  }

} // namespace api::camera
