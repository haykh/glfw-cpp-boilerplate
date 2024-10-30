#ifndef API_TEXTURE_H
#define API_TEXTURE_H

#include <string>

namespace api::texture {

  static unsigned int TextureId { 0 };

  class Texture {
    const unsigned int m_id;
    unsigned int       m_texture;
    bool               m_texture_generated { false };

  public:
    Texture() : m_id { TextureId++ } {}

    Texture(const std::string& path);

    ~Texture();

    void regenTexture(const std::string&);
    void use(unsigned int = 0) const;

    [[nodiscard]]
    auto id() const -> unsigned int {
      return m_id;
    }

    [[nodiscard]]
    auto texture() const -> unsigned int {
      return m_texture;
    }

    [[nodiscard]]
    auto generated() const -> bool {
      return m_texture_generated;
    }
  };

} // namespace api::texture

#endif // API_TEXTURE_H
