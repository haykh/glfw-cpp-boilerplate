#ifndef API_MESH_H
#define API_MESH_H

#include "global.h"

#include "api/material.h"
#include "api/object.h"
#include "api/prefabs.h"
#include "api/shader.h"
#include "utils/error.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <any>
#include <string>
#include <vector>

namespace api::mesh {
  using namespace utils;
  using namespace api::material;
  using namespace api::shader;
  using namespace api::object;

  static unsigned int MeshId { 0 };

  class Mesh : public Object {
    const unsigned int m_id;
    const std::string  m_name;

    std::vector<float>        m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<float>        m_uvCoords;
    unsigned int              m_vbo;

    vec_t       m_position { 0.0f };
    vec_t       m_scale { 1.0f };
    transform_t m_rotation { 1.0f };

    const vec_t*       m_bound_position { nullptr };
    const vec_t*       m_bound_scale { nullptr };
    const transform_t* m_bound_rotation { nullptr };

    Material* m_material { nullptr };

    bool m_buffers_generated { false };

    auto recalculate() const -> std::vector<float>;

  public:
    Mesh(const std::string&,
         const std::vector<float>&,
         const std::vector<unsigned int>&,
         const std::vector<float>&);

    Mesh(const std::string& name, const prefabs::Prefab& obj)
      : Mesh { name, obj.vertices, obj.indices, obj.uvCoords } {}

    Mesh(Mesh&& other) noexcept
      : m_id { MeshId++ }
      , m_name { std::move(other.m_name) }
      , m_vertices { std::move(other.m_vertices) }
      , m_indices { std::move(other.m_indices) }
      , m_uvCoords { std::move(other.m_uvCoords) }
      , m_vbo { other.m_vbo }
      , m_buffers_generated { other.m_buffers_generated } {}

    Mesh(const Mesh&) = delete;

    ~Mesh();

    void bindPosition(vec_t* const);
    void bindScale(vec_t* const);
    void bindRotation(transform_t* const);
    void set(const std::string&, std::any) override;

    void attachMaterial(Material*);
    void identifyMesh(const ShaderProgram&) const;

    // accessors
    [[nodiscard]]
    auto id() const -> unsigned int {
      return m_id;
    }

    [[nodiscard]]
    auto label() const -> std::string {
      return m_name + std::to_string(id());
    }

    [[nodiscard]]
    auto transform() const -> transform_t;

    [[nodiscard]]
    auto vbo() const -> unsigned int {
      return m_vbo;
    }

    [[nodiscard]]
    auto material() const -> Material* {
      if (m_material == nullptr) {
        raise::error("mesh material not set");
        return nullptr;
      } else {
        return m_material;
      }
    }

    // methods
    void regenBuffers();
    void render(const ShaderProgram&) const;

    void print() const;
  };

} // namespace api::mesh

#endif // API_MESH_H
