#ifndef API_MESH_H
#define API_MESH_H

#include "global.h"

#include "api/prefabs.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace api::mesh {

  class Mesh {
    const std::string         m_name;
    std::vector<float>        m_vertices;
    std::vector<unsigned int> m_indices;
    unsigned int              m_vbo;

    bool m_buffers_generated { false };

    color_t m_color { 0.33f };

    transform_t m_transform { 1.0f };

    auto recalculate() const -> std::vector<float>;

  public:
    Mesh(const std::string&,
         const std::vector<float>&,
         const std::vector<unsigned int>&);

    Mesh(const std::string& name, const prefabs::Prefab& obj)
      : Mesh { name, obj.vertices, obj.indices } {}

    Mesh(Mesh&& other) noexcept
      : m_name { std::move(other.m_name) }
      , m_vertices { std::move(other.m_vertices) }
      , m_indices { std::move(other.m_indices) }
      , m_vbo { other.m_vbo }
      , m_buffers_generated { other.m_buffers_generated }
      , m_color { other.m_color }
      , m_transform { other.m_transform } {}

    Mesh(const Mesh&) = delete;

    ~Mesh();

    // setters
    void setTransform(const transform_t& transform) {
      m_transform = transform;
    }

    void setColor(const color_t& color) {
      m_color = color;
    }

    // accessors
    [[nodiscard]]
    auto name() const -> const std::string& {
      return m_name;
    }

    [[nodiscard]]
    auto transform() const -> transform_t {
      return m_transform;
    }

    [[nodiscard]]
    auto color() const -> color_t {
      return m_color;
    }

    [[nodiscard]]
    auto vbo() const -> unsigned int {
      return m_vbo;
    }

    // methods
    void regenBuffers();
    void render() const;

    void print() const;
  };

} // namespace api::mesh

#endif // API_MESH_H
