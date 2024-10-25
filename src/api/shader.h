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
    std::string        m_original_source;
    bool               m_source_set { false };
    bool               m_compiled { false };

  public:
    Shader(const std::string&);
    ~Shader();

    void readShaderFromPath(const std::string&);
    void setOriginalShaderSource(const std::string&);
    void setShaderSource(const std::string&);

    void compile(bool = true);

    void recompile() {
      compile(false);
    }

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
    auto source() const -> const std::string& {
      return m_source;
    }

    [[nodiscard]]
    auto original_source() const -> const std::string& {
      return m_original_source;
    }

    [[nodiscard]]
    auto is_source_set() const -> bool {
      return m_source_set;
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
    void setShaderSources(const std::string&, const std::string&);

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

    // uniforms
    void setUniform1f(const std::string&, float) const;
    void setUniform1i(const std::string&, int) const;
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
