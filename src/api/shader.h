#ifndef API_SHADER_H
#define API_SHADER_H

#include <glad/gl.h>

#include <glm/glm.hpp>

#include <string>

namespace api::shader {

  template <GLenum S>
  class Shader {
    const std::string  m_label;
    const unsigned int m_id;
    std::string        m_source;
    std::string        m_source_in { "" };
    std::string        m_source_in_fname;
    bool               m_compiled { false };

  public:
    Shader(const std::string&);
    ~Shader();

    void readShaderFromPath(const std::string&);

    void saveShaderSource() const;
    void replaceString(const std::string&, const std::string&);

    void compile(bool = true);

    void recompile() {
      compile(false);
    }

    void setCompiled(bool compiled) {
      m_compiled = compiled;
    }

    // accessors
    [[nodiscard]]
    auto id() const -> unsigned int {
      return m_id;
    }

    [[nodiscard]]
    auto label() const -> std::string {
      return m_label;
    }

    [[nodiscard]]
    auto source() const -> std::string {
      return m_source;
    }

    [[nodiscard]]
    auto source_in() const -> std::string {
      return m_source_in;
    }

    [[nodiscard]]
    auto is_compiled() const -> bool {
      return m_compiled;
    }
  };

  class ShaderProgram {
    const std::string          m_label;
    const unsigned int         m_id;
    bool                       m_linked { false };
    Shader<GL_VERTEX_SHADER>   m_vertexShader;
    Shader<GL_FRAGMENT_SHADER> m_fragmentShader;

  public:
    ShaderProgram(const std::string&);
    ~ShaderProgram();

    void readShadersFromPaths(const std::string&, const std::string&);

    void compile(bool = true);

    void recompile() {
      compile(false);
    }

    void link(bool = true);

    void relink() {
      link(false);
    }

    void use() const;
    void print() const;

    void setLinked(bool linked) {
      m_linked = linked;
    }

    void setCompiled(bool compiled) {
      m_vertexShader.setCompiled(compiled);
      m_fragmentShader.setCompiled(compiled);
    }

    // uniforms
    void setUniform1f(const std::string&, float) const;
    void setUniform1i(const std::string&, int) const;
    void setUniform1ui(const std::string&, unsigned int) const;
    void setUniform1b(const std::string&, bool) const;
    void setUniform3fv(const std::string&, const glm::vec3&) const;
    void setUniformMatrix4fv(const std::string&, const glm::mat4&) const;

    // accessors
    [[nodiscard]]
    auto id() const -> unsigned int {
      return m_id;
    }

    [[nodiscard]]
    auto label() const -> const std::string& {
      return m_label;
    }

    [[nodiscard]]
    auto is_linked() const -> bool {
      return m_linked;
    }

    [[nodiscard]]
    auto vertexShader() -> Shader<GL_VERTEX_SHADER>& {
      return m_vertexShader;
    }

    [[nodiscard]]
    auto fragmentShader() -> Shader<GL_FRAGMENT_SHADER>& {
      return m_fragmentShader;
    }
  };

} // namespace api::shader

#endif // API_SHADER_H
