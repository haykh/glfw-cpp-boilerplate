#include "shaders.h"

#include "utils/error.h"
#include "utils/log.h"
#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace shaders {

  auto createShaderProgram(const std::string& name,
                           const char*        vertexShaderSource,
                           const char* fragmentShaderSource) -> unsigned int {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    checkCompilation(vertexShader, name + " vertex");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    checkCompilation(fragmentShader, name + " fragment");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkLinking(shaderProgram, name + " shader");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
  }

  auto createVAO(const std::string& name,
                 const float*       vertices,
                 std::size_t        size) -> unsigned int {
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
  }

  void checkCompilation(unsigned int id, const std::string& shader) {
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
      char infoLog[512];
      glGetShaderInfoLog(id, 512, nullptr, infoLog);
      raise::error(
        shader + " shader compilation failed : " + std::string(infoLog));
    } else {
      log::log(log::SUCCESS, shader + " shader compiled successfully");
    }
  }

  void checkLinking(unsigned int id, const std::string& program) {
    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
      char infoLog[512];
      glGetShaderInfoLog(id, 512, nullptr, infoLog);
      raise::error(program + " program link failed : " + std::string(infoLog));
    } else {
      log::log(log::SUCCESS, program + " program linked successfully");
    }
  }

} // namespace shaders
