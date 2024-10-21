#ifndef API_SHADER_H
#define API_SHADER_H

#include <glad/gl.h>

#include <string>

namespace api {

  template <GLenum S>
  class Shader {
    const unsigned int m_id;
    const std::string  m_label;
    std::string        m_source;
    bool               m_source_set { false };
    bool               m_compiled { false };

  public:
    Shader(const std::string&);
    ~Shader();

    void readShaderFromPath(const std::string&);
    void setShaderSource(const std::string& source);

    void compile();

    // accessors
    [[nodiscard]]
    auto id() const noexcept -> unsigned int {
      return m_id;
    }

    [[nodiscard]]
    auto label() const noexcept -> const std::string& {
      return m_label;
    }

    [[nodiscard]]
    auto source() const noexcept -> const std::string& {
      return m_source;
    }

    [[nodiscard]]
    auto is_source_set() const noexcept -> bool {
      return m_source_set;
    }

    [[nodiscard]]
    auto is_compiled() const noexcept -> bool {
      return m_compiled;
    }
  };

  class ShaderProgram {
    const unsigned int         m_id;
    const std::string          m_label;
    bool                       m_linked { false };
    Shader<GL_VERTEX_SHADER>   m_vertexShader;
    Shader<GL_FRAGMENT_SHADER> m_fragmentShader;

  public:
    ShaderProgram(const std::string&);
    ~ShaderProgram();

    void readShadersFromPaths(const std::string& vertex,
                              const std::string& fragment);
    void setShaderSources(const std::string& vertex,
                          const std::string& fragment);

    void compile();
    void link();
    void use() const;

    // uniforms
    void setUniform1f(const std::string& name, float value);
    void setUniform1i(const std::string& name, int value);
    void setUniform1b(const std::string& name, bool value);

    // accessors
    [[nodiscard]]
    auto id() const noexcept -> unsigned int {
      return m_id;
    }

    [[nodiscard]]
    auto label() const noexcept -> const std::string& {
      return m_label;
    }

    [[nodiscard]]
    auto is_linked() const noexcept -> bool {
      return m_linked;
    }
  };

} // namespace api

#endif // API_SHADER_H
