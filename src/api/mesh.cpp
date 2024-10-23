#include "mesh.h"

#include <glad/gl.h>

namespace api {

  Mesh::Mesh(const std::string&               name,
             const std::vector<float>&        vertices,
             const std::vector<unsigned int>& indices)
    : m_name { name }
    , m_vertices { vertices }
    , m_indices { indices } {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 m_vertices.size() * sizeof(float),
                 m_vertices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 m_indices.size() * sizeof(unsigned int),
                 m_indices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  Mesh::~Mesh() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
  }

  void Mesh::render() const {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES,
                   static_cast<GLsizei>(m_indices.size()),
                   GL_UNSIGNED_INT,
                   0);
  }

} // namespace api
