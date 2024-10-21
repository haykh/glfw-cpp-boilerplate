#include "engine.h"

#include "utils/log.h"
#include "utils/shaders.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace engine {

  Engine::Engine(float scale,
                 int   win_width,
                 int   win_height,
                 float col_bg[4],
                 bool  resizable) {
    m_window = std::make_unique<Window>((int)(win_width * scale / 2.0f),
                                        (int)(win_height * scale / 2.0f),
                                        "engine_window",
                                        1,
                                        col_bg,
                                        resizable);
  }

  void Engine::renderLoop() {
    const char* vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";

    const char* fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\0";
    auto shaderProgram = shaders::createShaderProgram("triangle",
                                                      vertexShaderSource,
                                                      fragmentShaderSource);

    // configure vertex data
    float vertices[] = {
      -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f
    };
    auto VAO = shaders::createVAO("triangle", vertices, sizeof(vertices));

    log::log(log::INFO, "starting render loop");

    while (!m_window->windowShouldClose()) {
      m_window->processInput();
      m_window->clear();

      // render stuff here >>>
      glUseProgram(shaderProgram);
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      // finish rendering stuff <<<

      m_window->unuse();
    }
  }

} // namespace engine
