#include "mesh.h"

#include "utils/error.h"

#include <glad/gl.h>

#include <cstdio>

namespace api::mesh {
  using namespace utils;

  Mesh::Mesh(const std::string&               name,
             const std::vector<float>&        vertices,
             const std::vector<unsigned int>& indices)
    : m_name { name }
    , m_vertices { vertices }
    , m_indices { indices } {}

  Mesh::~Mesh() {
    glDeleteBuffers(1, &m_vbo);
  }

  void Mesh::regenBuffers() {
    const auto vertices = recalculate();
    glGenBuffers(1, &m_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_buffers_generated = true;
  }

  auto Mesh::recalculate() const -> std::vector<float> {
    std::vector<float> vertices(m_indices.size() * 6, 0.0);
    for (auto tidx = 0u; tidx < m_indices.size(); tidx += 3) {
      const auto vidx1 = m_indices[tidx + 0] * 3;
      const auto vidx2 = m_indices[tidx + 1] * 3;
      const auto vidx3 = m_indices[tidx + 2] * 3;

      const auto normal = vec_t(
        -m_vertices[vidx2 + 2] * m_vertices[vidx3 + 1] +
          m_vertices[vidx1 + 2] *
            (-m_vertices[vidx2 + 1] + m_vertices[vidx3 + 1]) +
          m_vertices[vidx1 + 1] *
            (m_vertices[vidx2 + 2] - m_vertices[vidx3 + 2]) +
          m_vertices[vidx2 + 1] * m_vertices[vidx3 + 2],
        m_vertices[vidx1 + 2] *
            (m_vertices[vidx2 + 0] - m_vertices[vidx3 + 0]) +
          m_vertices[vidx2 + 2] * m_vertices[vidx3 + 0] -
          m_vertices[vidx2 + 0] * m_vertices[vidx3 + 2] +
          m_vertices[vidx1 + 0] *
            (-m_vertices[vidx2 + 2] + m_vertices[vidx3 + 2]),
        -m_vertices[vidx2 + 1] * m_vertices[vidx3 + 0] +
          m_vertices[vidx1 + 1] *
            (-m_vertices[vidx2 + 0] + m_vertices[vidx3 + 0]) +
          m_vertices[vidx1 + 0] *
            (m_vertices[vidx2 + 1] - m_vertices[vidx3 + 1]) +
          m_vertices[vidx2 + 0] * m_vertices[vidx3 + 1]);
      for (auto vidx = 0u; vidx < 3; vidx++) {
        vertices[(tidx + vidx) * 6 + 0] =
          m_vertices[m_indices[tidx + vidx] * 3 + 0];
        vertices[(tidx + vidx) * 6 + 1] =
          m_vertices[m_indices[tidx + vidx] * 3 + 1];
        vertices[(tidx + vidx) * 6 + 2] =
          m_vertices[m_indices[tidx + vidx] * 3 + 2];
        vertices[(tidx + vidx) * 6 + 3] = normal.x;
        vertices[(tidx + vidx) * 6 + 4] = normal.y;
        vertices[(tidx + vidx) * 6 + 5] = normal.z;
      }
    }
    return vertices;
  }

  void Mesh::render() const {
    if (!m_buffers_generated) {
      raise::error("buffers not generated for mesh: " + m_name);
    }
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_indices.size()));
  }

  void Mesh::print() const {
    printf("%s : nvert [%ld] : nind [%ld] -- %s",
           m_name.c_str(),
           m_vertices.size(),
           m_indices.size(),
           m_buffers_generated ? "✓" : "✗");
  }

} // namespace api::mesh
