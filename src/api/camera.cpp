#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace api {

  auto Camera::view() const -> glm::mat4 {
    if (!m_stare) {
      return glm::lookAt(m_position, m_position + m_front, m_up);
    } else {
      return glm::lookAt(m_position, m_target, m_up);
    }
  }

  auto Camera::project() const -> glm::mat4 {
    return glm::perspective(glm::radians(fov()), aspect(), zNear(), zFar());
  }

  // const glm::vec3& target

} // namespace api
