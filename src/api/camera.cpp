#include "camera.h"

#include "utils/error.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <any>
#include <cstdio>

namespace api::camera {
  using namespace utils;

  void Camera::set(const std::string& key, std::any value) {
    if (key == "type") {
      m_type = std::any_cast<CameraType>(value);
    } else if (key == "position") {
      m_position = std::any_cast<pos_t>(value);
    } else if (key == "fov") {
      m_fov = std::any_cast<float>(value);
    } else if (key == "aspect") {
      m_aspect = std::any_cast<float>(value);
    } else if (key == "zNear") {
      m_zNear = std::any_cast<float>(value);
    } else if (key == "zFar") {
      m_zFar = std::any_cast<float>(value);
    } else if (key == "yaw") {
      m_yaw = std::any_cast<float>(value);
    } else if (key == "pitch") {
      m_pitch = std::any_cast<float>(value);
    } else if (key == "roll") {
      m_roll = std::any_cast<float>(value);
    } else {
      raise::error("invalid key for camera: " + key);
    }
  }

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
    cam->set("yaw", cam->yaw() + xOffset);
    cam->set("pitch", cam->pitch() + yOffset);
    if (cam->pitch() > 89.0f) {
      cam->set("pitch", 89.0f);
    } else if (cam->pitch() < -89.0f) {
      cam->set("pitch", -89.0f);
    }
    lastX = xPos;
    lastY = yPos;
  }

  void Camera::processKeyboardInput(GLFWwindow* window, float dt) {
    const auto norm_speed = speed() * dt;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      set("position", position() + norm_speed * horizontalFront());
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      set("position", position() - norm_speed * horizontalFront());
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      set("position",
          position() -
            glm::normalize(glm::cross(horizontalFront(), up())) * norm_speed);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      set("position",
          position() +
            glm::normalize(glm::cross(horizontalFront(), up())) * norm_speed);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      set("position", position() + 0.5f * norm_speed * WorldUp);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
      set("position", position() - 0.5f * norm_speed * WorldUp);
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
