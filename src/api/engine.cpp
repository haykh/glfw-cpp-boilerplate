#include "engine.h"

#include "api/shader.h"
#include "utils/log.h"
#include <GL/gl.h>
#include <map>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace engine {

  auto createVertexBuffers(const std::string&  name,
                           const float*        vertices,
                           std::size_t         v_size,
                           const unsigned int* indices,
                           std::size_t         i_size) noexcept
    -> std::map<std::string, unsigned int> {
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v_size, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_size, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return {
      {"VAO", VAO},
      {"VBO", VBO},
      {"EBO", EBO}
    };
  }

  Engine::Engine(float scale,
                 int   win_width,
                 int   win_height,
                 float col_bg[4],
                 bool  resizable) {
    m_window = std::make_unique<api::Window>((int)(win_width * scale / 2.0f),
                                             (int)(win_height * scale / 2.0f),
                                             "engine_window",
                                             1,
                                             col_bg,
                                             resizable);
  }

  void Engine::renderLoop() {
    api::ShaderProgram shader("rectangle");
    shader.readShadersFromPaths("src/shaders/test.vert",
                                "src/shaders/test.frag");
    shader.compile();
    shader.link();

    float        vertices[] = { 0.5f,  0.5f,  0.0f, 0.5f,  -0.5f, 0.0f,
                                -0.5f, -0.5f, 0.0f, -0.5f, 0.5f,  0.0f };
    unsigned int indices[]  = { 0, 1, 3, 1, 2, 3 };
    auto         buffers    = createVertexBuffers("triangle",
                                       vertices,
                                       sizeof(vertices),
                                       indices,
                                       sizeof(indices));

    glBindVertexArray(buffers.at("VAO"));

    log::log(log::INFO, "starting render loop");
    while (!m_window->windowShouldClose()) {
      m_window->processInput();
      m_window->clear();

      // render stuff here >>>
      shader.use();
      shader.setUniform1f("time", (float)glfwGetTime());
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      // finish rendering stuff <<<

      m_window->unuse();
    }

    glDeleteVertexArrays(1, &buffers.at("VAO"));
    glDeleteBuffers(1, &buffers.at("VBO"));
    glDeleteBuffers(1, &buffers.at("EBO"));
  }

} // namespace engine
