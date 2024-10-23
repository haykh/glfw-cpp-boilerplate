#include "mesh.h"

#include <glad/gl.h>

// #include <utility>

namespace api {

  Mesh::Mesh(const std::string&               name,
             const std::vector<float>&        vertices,
             const std::vector<unsigned int>& indices)
    : m_name { name }
    , m_vertices { vertices }
    , m_indices { indices } {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    regenBuffers();
  }

  Mesh::~Mesh() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
  }

  void Mesh::regenBuffers() const {
    const auto vertices = recalculate();

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glBindVertexArray(m_vao);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  auto Mesh::recalculate() const -> std::vector<float> {
    std::vector<float> vertices(m_indices.size() * 6, 0.0);
    for (auto tidx = 0u; tidx < m_indices.size(); tidx += 3) {
      const auto vidx1 = m_indices[tidx + 0] * 3;
      const auto vidx2 = m_indices[tidx + 1] * 3;
      const auto vidx3 = m_indices[tidx + 2] * 3;

      const auto normal = -glm::vec3(
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
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_indices.size()));
  }

} // namespace api
