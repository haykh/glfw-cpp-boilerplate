#include "shader.h"

#include "utils/error.h"
#include "utils/log.h"

#include <glad/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <string>

namespace api::shader {
  using namespace utils;

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
  void Shader<S>::setOriginalShaderSource(const std::string& source) {
    m_original_source = source;
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
    log::log(log::DEBUG, "reading shader file : " + path);
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
    setOriginalShaderSource(shader_src);
  }

  template <GLenum S>
  void Shader<S>::compile(bool errorIfCompiled) {
    if (is_compiled() && errorIfCompiled) {
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
      log::log(log::INFO, "shader source : \n" + source());
      raise::error(
        label() + " shader compilation failed : " + std::string(infoLog));
    } else {
      log::log(log::SUCCESS, label() + " shader compiled successfully");
    }
    m_compiled = true;
  }

  ShaderProgram::ShaderProgram(const std::string& label)
    : m_label { label }
    , m_id { glCreateProgram() }
    , m_vertexShader { label + " vertex" }
    , m_fragmentShader { label + " fragment" } {}

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

  void ShaderProgram::compile(bool errorIfCompiled) {
    m_vertexShader.compile(errorIfCompiled);
    m_fragmentShader.compile(errorIfCompiled);
  }

  void ShaderProgram::link(bool errorIfLinked) {
    if (is_linked() && errorIfLinked) {
      raise::error("shader program already linked");
    } else if (!m_vertexShader.is_compiled() ||
               !m_fragmentShader.is_compiled()) {
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

  void ShaderProgram::setUniform1f(const std::string& name, float value) const {
    log::log(log::DEBUG,
             "setting uniform " + name + " to " + std::to_string(value));
    glUniform1f(glGetUniformLocation(id(), name.c_str()), value);
  }

  void ShaderProgram::setUniform1i(const std::string& name, int value) const {
    log::log(log::DEBUG,
             "setting uniform " + name + " to " + std::to_string(value));
    glUniform1i(glGetUniformLocation(id(), name.c_str()), value);
  }

  void ShaderProgram::setUniform1b(const std::string& name, bool value) const {
    log::log(log::DEBUG,
             "setting uniform " + name + " to " + std::to_string(value));
    glUniform1i(glGetUniformLocation(id(), name.c_str()), value);
  }

  void ShaderProgram::setUniform3fv(const std::string& name,
                                    const glm::vec3&   value) const {
    log::log(log::DEBUG,
             "setting uniform " + name + " to " + std::to_string(value.x) +
               " " + std::to_string(value.y) + " " + std::to_string(value.z));
    glUniform3fv(glGetUniformLocation(id(), name.c_str()),
                 1,
                 glm::value_ptr(value));
  }

  void ShaderProgram::setUniformMatrix4fv(const std::string& name,
                                          const glm::mat4&   value) const {
    glUniformMatrix4fv(glGetUniformLocation(id(), name.c_str()),
                       1,
                       GL_FALSE,
                       glm::value_ptr(value));
  }

  void ShaderProgram::print() const {
    printf("%s %u : %s\n", m_label.c_str(), m_id, m_linked ? "✓" : "✗");
  }

  template class Shader<GL_VERTEX_SHADER>;
  template class Shader<GL_FRAGMENT_SHADER>;

} // namespace api::shader
