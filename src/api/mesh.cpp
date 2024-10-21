#include "mesh.h"

#include <glad/gl.h>

namespace api {

  Mesh::~Mesh() {
    const auto vao_buff = buffer("VAO");
    glDeleteVertexArrays(1, &vao_buff);
    const auto vbo_buff = buffer("VBO");
    glDeleteBuffers(1, &vbo_buff);
    const auto ebo_buff = buffer("EBO");
    glDeleteBuffers(1, &ebo_buff);
  }

  void Mesh::genBuffers() {
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 m_vertices.size() * sizeof(float),
                 m_vertices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 m_indices.size() * sizeof(unsigned int),
                 m_indices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    m_buffers = {
      {"VAO", VAO},
      {"VBO", VBO},
      {"EBO", EBO}
    };
  }

  void Mesh::bind() const {
    glBindVertexArray(buffer("VAO"));
  }

  void Mesh::render() const {
    glDrawElements(GL_TRIANGLES,
                   static_cast<GLsizei>(m_indices.size()),
                   GL_UNSIGNED_INT,
                   0);
  }

} // namespace api
