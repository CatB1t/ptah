#include "core/texture2d.hpp"

#include <glad/gl.h>

namespace ptah {

unsigned int Texture2D::m_ToGlFormat(ImageFormat format) {
  switch (format) {
    case ImageFormat::RGB:
      return GL_RGB;
    case ImageFormat::RGBA:
      return GL_RGBA;
  }
  return GL_RGB;
}

Texture2D::Texture2D(Image& image) {
  unsigned int temp;
  glGenTextures(1, &temp);
  m_handle.Set(temp);
  glBindTexture(GL_TEXTURE_2D, m_handle.Id());
  auto format = m_ToGlFormat(image.format);
  glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, GL_FALSE,
               format, GL_UNSIGNED_BYTE, image.data.data());
  glGenerateMipmap(GL_TEXTURE_2D);
  m_SetTextureParams();
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::m_SetTextureParams() {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture2D::Bind(unsigned int slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, m_handle.Id());
}
}  // namespace ptah