#include <over/core/opengl/Texture.hpp>

#include <stdexcept>

#include <over/core/opengl/wrappers/Exception.hpp>

#include <fmt/core.h>

namespace over::gl {
GLenum Texture::GetFormat(usize channels) {
  GLenum format = GL_RGB;
  switch (channels) {
    case 0:
      format = GL_RGB;
      break;
    case 1:
      format = GL_RED;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      throw std::runtime_error(fmt::format(
          "Unknown texture format, number of channels: {}", channels));
  }

  return format;
}

void Texture::Activate(GLenum target) {
  glthrow(glActiveTexture(target));
}
}  // namespace over::gl
