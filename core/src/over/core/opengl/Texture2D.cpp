#include <over/core/opengl/Texture2D.hpp>

#include <cassert>
#include <stdexcept>
#include <vector>

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>

namespace over {
Texture2D::Texture2D() noexcept
    : _id(0), _width(0), _height(0), _format(static_cast<uint32>(GL_RGB)) {}

Texture2D::Texture2D(uint32 width, uint32 height, const ubyte* data,
                     GLenum format)
    : _width(width), _height(height), _format(static_cast<uint32>(format)) {
  Setup(data, format);
}

Texture2D::Texture2D(const Texture2D& other) : Texture2D() {
  *this = other;
}

Texture2D::Texture2D(Texture2D&& other) noexcept : Texture2D() {
  *this = std::move(other);
}

Texture2D& Texture2D::operator=(const Texture2D& other) {
  if (this == &other) {
    return *this;
  }

  assert(_id != other._id);

  _width = other._width;
  _height = other._height;
  _format = other._format;

  // copy image
  std::vector<ubyte> data(_width * _height, 0);
  other.Bind();
  glGetTexImage(GL_TEXTURE_2D, 0, other._format, GL_UNSIGNED_BYTE, data.data());
  other.Unbind();

  Setup(data.data(), other._format);

  return *this;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  _id = std::exchange(other._id, 0);
  _width = std::exchange(other._width, 0);
  _height = std::exchange(other._height, 0);
  _format = std::exchange(other._format, GL_RGB);

  return *this;
}

Texture2D::~Texture2D() {
  FreeGPU();
}

void Texture2D::Setup(const ubyte* data, GLenum format) {
  if (_id == 0) {
    glGenTextures(1, &_id);
  }

  if (_id == 0) {

    throw std::runtime_error("error while creating texture object");
  }

  ToGPU(data, format);

  // TODO: add parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  Unbind();
}

void Texture2D::Bind() const noexcept {
  assert(_id != 0);
  glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture2D::Unbind() const noexcept {
  assert(_id != 0);
  // TODO: assert binded texture is _id
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::ToGPU(const ubyte* data, GLenum format, bool unbind) {
  Bind();

  glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLenum>(_format),
               static_cast<GLsizei>(_width), static_cast<GLsizei>(_height), 0,
               format, GL_UNSIGNED_BYTE, data);

  if (unbind) {
    Unbind();
  }
}

void Texture2D::FreeGPU() noexcept {
  assert(_id != 0);
  glDeleteTextures(1, &_id);
  _id = 0;
}

}  // namespace over
