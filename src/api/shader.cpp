#include "shader.h"

#include "utils/error.h"
#include "utils/log.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>

namespace api {
  namespace log   = utils;
  namespace raise = utils;

  template <GLenum S>
  Shader<S>::Shader(const std::string& label)
    : m_label { label }
    , m_id { glCreateShader(S) } {}

  template <GLenum S>
  Shader<S>::~Shader() {
    if (is_compiled()) {
      glDeleteShader(id());
    }
  }

  template <GLenum S>
  void Shader<S>::setShaderSource(const std::string& source) {
    m_source         = source;
    m_source_set     = true;
    const auto src_c = m_source.c_str();
    glShaderSource(id(), 1, &src_c, nullptr);
  }

  template <GLenum S>
  void Shader<S>::readShaderFromPath(const std::string& path) {
    if (is_source_set()) {
      raise::error("shader source already set");
    }
    // check filenames
    if constexpr (S == GL_VERTEX_SHADER) {
      if (path.find(".vert") == std::string::npos) {
        raise::error("vertex shader must have .vert extension");
      }
    } else if constexpr (S == GL_FRAGMENT_SHADER) {
      if (path.find(".frag") == std::string::npos) {
        raise::error("fragment shader must have .frag extension");
      }
    }
    std::string   shader_src;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      file.open(path);
      std::stringstream stream;
      stream << file.rdbuf();
      file.close();
      shader_src = stream.str();
    } catch (const std::ifstream::failure& e) {
      raise::error("failed to read shader file : " + std::string(e.what()));
    }
    setShaderSource(shader_src);
  }

  template <GLenum S>
  void Shader<S>::compile() {
    if (is_compiled()) {
      raise::error("shader already compiled");
    } else if (!is_source_set()) {
      raise::error("shader source not set");
    }
    const auto src_c = source().c_str();
    glShaderSource(id(), 1, &src_c, nullptr);
    glCompileShader(id());

    // check for compilation errors
    int success;
    glGetShaderiv(id(), GL_COMPILE_STATUS, &success);
    if (!success) {
      char infoLog[512];
      glGetShaderInfoLog(id(), 512, nullptr, infoLog);
      raise::error(
        label() + " shader compilation failed : " + std::string(infoLog));
    } else {
      log::log(log::SUCCESS, label() + " shader compiled successfully");
    }
    m_compiled = true;
  }

  ShaderProgram::ShaderProgram(const std::string& label)
    : m_label { label }
    , m_vertexShader { label + " vertex" }
    , m_fragmentShader { label + " fragment" }
    , m_id { glCreateProgram() } {}

  ShaderProgram::~ShaderProgram() {
    if (is_linked()) {
      glDeleteProgram(id());
    }
  }

  void ShaderProgram::readShadersFromPaths(const std::string& vertex_path,
                                           const std::string& fragment_path) {
    m_vertexShader.readShaderFromPath(vertex_path);
    m_fragmentShader.readShaderFromPath(fragment_path);
  }

  void ShaderProgram::setShaderSources(const std::string& vertex,
                                       const std::string& fragment) {
    m_vertexShader.setShaderSource(vertex);
    m_fragmentShader.setShaderSource(fragment);
  }

  void ShaderProgram::compile() {
    m_vertexShader.compile();
    m_fragmentShader.compile();
  }

  void ShaderProgram::link() {
    if (is_linked()) {
      raise::error("shader program already linked");
    } else if (not m_vertexShader.is_compiled() ||
               not m_fragmentShader.is_compiled()) {
      raise::error(
        "vertex and fragment shaders must be compiled before linking");
    }
    glAttachShader(id(), m_vertexShader.id());
    glAttachShader(id(), m_fragmentShader.id());
    glLinkProgram(id());

    // check for linking errors
    int success;
    glGetProgramiv(id(), GL_LINK_STATUS, &success);
    if (!success) {
      char infoLog[512];
      glGetShaderInfoLog(id(), 512, nullptr, infoLog);
      raise::error(label() + " program link failed : " + std::string(infoLog));
    } else {
      log::log(log::SUCCESS, label() + " program linked successfully");
    }
    m_linked = true;
  }

  void ShaderProgram::use() const {
    if (is_linked()) {
      glUseProgram(id());
    } else {
      raise::error("shader program not linked");
    }
  }

  void ShaderProgram::setUniform1f(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(id(), name.c_str()), value);
  }

  void ShaderProgram::setUniform1i(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(id(), name.c_str()), value);
  }

  void ShaderProgram::setUniform1b(const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(id(), name.c_str()), value);
  }

  template class Shader<GL_VERTEX_SHADER>;
  template class Shader<GL_FRAGMENT_SHADER>;

} // namespace api