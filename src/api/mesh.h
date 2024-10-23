#ifndef API_MESH_H
#define API_MESH_H

#include "api/prefabs.h"

#include <glm/glm.hpp>

#include <string>
// #include <utility>
#include <vector>

namespace api {

  class Mesh {
    const std::string         m_name;
    std::vector<float>        m_vertices;
    std::vector<unsigned int> m_indices;
    unsigned int              m_vao;
    unsigned int              m_vbo;
    unsigned int              m_ebo;

    glm::vec3 m_color { 0.33f };

    glm::mat4 m_transform { 1.0f };

  public:
    Mesh(const std::string&,
         const std::vector<float>&,
         const std::vector<unsigned int>&);

    Mesh(const std::string& name, const prefabs::Prefab& obj)
      : Mesh { name, obj.vertices, obj.indices } {}

    ~Mesh();

    // setters
    void setTransform(const glm::mat4& transform) {
      m_transform = transform;
    }

    void setColor(const glm::vec3& color) {
      m_color = color;
    }

    // accessors
    [[nodiscard]]
    auto name() const -> const std::string& {
      return m_name;
    }

    [[nodiscard]]
    auto transform() const -> glm::mat4 {
      return m_transform;
    }

    [[nodiscard]]
    auto color() const -> glm::vec3 {
      return m_color;
    }

    [[nodiscard]]
    auto vao() const -> unsigned int {
      return m_vao;
    }

    [[nodiscard]]
    auto vbo() const -> unsigned int {
      return m_vbo;
    }

    [[nodiscard]]
    auto ebo() const -> unsigned int {
      return m_ebo;
    }

    // methods
    auto recalculate() const -> std::vector<float>;

    void regenBuffers() const;
    void render() const;
  };

} // namespace api

#endif // API_MESH_H
