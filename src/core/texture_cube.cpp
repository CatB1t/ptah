#include "core/texture_cube.hpp"

#include <glad/gl.h>

#include <format>

#include "utils/file_loading.hpp"
#include "utils/logger.hpp"

namespace ptah {
void TextureCube::m_SetTextureParams(TextureProps props) {
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                  props.to_gl(props.min_filter));
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
                  props.to_gl(props.mag_filter));
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                  props.to_gl(props.s_wrap));
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                  props.to_gl(props.t_wrap));
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                  props.to_gl(props.r_wrap));
}

TextureCube::TextureCube(const std::filesystem::path& textures_dir) {
  unsigned int tmp;
  glGenTextures(1, &tmp);
  m_handle.Set(tmp);
  std::string ext = "png";
  if (std::filesystem::exists(textures_dir / "px.jpg")) {
    ext = "jpg";
  } else if (std::filesystem::exists(textures_dir / "px.jpeg")) {
    ext = "jpeg";
  }

  constexpr const char* textures[6]{
      "px", "nx", "py", "ny", "pz", "nz",
  };

  constexpr int tex_targets[6]{
      GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
      GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
      GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
  };

  glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle.Id());
  TextureProps props{};
  props.min_filter = MinFilter::Linear;
  props.mag_filter = MagFilter::Linear;
  props.s_wrap = AxisWrap::CLAMP_EDGE;
  props.t_wrap = AxisWrap::CLAMP_EDGE;
  props.r_wrap = AxisWrap::CLAMP_EDGE;
  m_SetTextureParams(props);
  for (int i = 0; i < 6; i++) {
    auto img = utils::load_image(textures_dir /
                                 std::format("{}.{}", textures[i], ext));
    if (!img) {
      continue;
    }
    auto format = img->format == ImageFormat::RGB ? GL_RGB : GL_RGBA;
    glTexImage2D(tex_targets[i], 0, format, img->width, img->height, GL_FALSE,
                 format, GL_UNSIGNED_BYTE, img->data.data());
  }
}

void TextureCube::Bind(unsigned int slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle.Id());
}
}  // namespace ptah
