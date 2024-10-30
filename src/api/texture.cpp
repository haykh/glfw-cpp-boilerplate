#include "texture.h"

#include "utils/error.h"
#include "utils/log.h"

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <string>

namespace api::texture {
  using namespace utils;

  Texture::Texture(const std::string& path) : m_id { TextureId++ } {
    regenTexture(path);
  }

  Texture::~Texture() {
    if (generated()) {
      glDeleteTextures(1, &m_texture);
    }
  }

  void Texture::regenTexture(const std::string& path) {
    if (generated()) {
      log::log(log::WARNING, "Texture already generated, deleting");
      glDeleteTextures(1, &m_texture);
      m_texture_generated = false;
    }
    glGenTextures(1, &m_texture);

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data) {
      GLenum format;
      if (channels == 1) {
        format = GL_RED;
      } else if (channels == 3) {
        format = GL_RGB;
      } else if (channels == 4) {
        format = GL_RGBA;
      } else {
        raise::error("Unknown texture format");
        return;
      }
      glBindTexture(GL_TEXTURE_2D, m_texture);
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   format,
                   width,
                   height,
                   0,
                   format,
                   GL_UNSIGNED_BYTE,
                   data);
      glGenerateMipmap(GL_TEXTURE_2D);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D,
                      GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
      raise::error("Failed to load texture");
    }
    stbi_image_free(data);
    m_texture_generated = true;
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void Texture::use(unsigned int i) const {
    if (!generated()) {
      raise::error("Texture not generated");
    } else {
      if (i == 0) {
        glActiveTexture(GL_TEXTURE0);
      } else if (i == 1) {
        glActiveTexture(GL_TEXTURE1);
      } else if (i == 2) {
        glActiveTexture(GL_TEXTURE2);
      } else if (i == 3) {
        glActiveTexture(GL_TEXTURE3);
      } else if (i == 4) {
        glActiveTexture(GL_TEXTURE4);
      } else {
        raise::error("Texture unit not supported");
      }
      glBindTexture(GL_TEXTURE_2D, m_texture);
    }
  }

} // namespace api::texture
