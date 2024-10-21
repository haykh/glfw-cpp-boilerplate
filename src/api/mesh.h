#ifndef API_MESH_H
#define API_MESH_H

#include "api/prefabs.h"
#include "utils/error.h"

#include <glm/glm.hpp>

#include <map>
#include <string>
#include <vector>

namespace api {
  namespace raise = utils;

  class Mesh {
    const std::string                   m_name;
    std::vector<float>                  m_vertices;
    std::vector<unsigned int>           m_indices;
    std::map<std::string, unsigned int> m_buffers;

    glm::mat4 m_transform { 1.0f };

  public:
    Mesh(const std::string&               name,
         const std::vector<float>&        vertices,
         const std::vector<unsigned int>& indices)
      : m_name { name }
      , m_vertices { vertices }
      , m_indices { indices } {}

    Mesh(const std::string& name, const prefabs::Prefab& obj)
      : Mesh { name, obj.vertices, obj.indices } {}

    ~Mesh();

    // setters
    void setTransform(const glm::mat4& transform) {
      m_transform = transform;
    }

    // accessors
    [[nodiscard]]
    auto buffer(const std::string& name) const -> unsigned int {
      if (m_buffers.find(name) == m_buffers.end()) {
        raise::error("buffer not found: " + std::string(name));
      }
      return m_buffers.at(name);
    }

    [[nodiscard]]
    auto name() const -> const std::string& {
      return m_name;
    }

    [[nodiscard]]
    auto transform() const -> glm::mat4 {
      return m_transform;
    }

    // methods
    void genBuffers();
    void bind() const;
    void render() const;
  };

} // namespace api

#endif // API_MESH_H
