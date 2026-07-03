#include <over/core/opengl/Texture2D.hpp>

#include <cassert>
#include <stdexcept>
#include <vector>

#include <over/core/Includes.hpp>
#include <over/core/Mesh.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/Binded.hpp>

namespace over {
Texture2D::Texture2D() noexcept : _width(0), _height(0), _texture() {}

Texture2D::Texture2D(usize width, usize height, GLenum format, GLenum type,
                     const void* data)
    : _width(width), _height(height), _texture() {
  Setup(width, height, format, type, data);
}

Texture2D::operator MeshTexture() const {
  return MeshTexture(_texture.As<gl::TextureTarget::TEXTURE_2D>(),
                     MeshTexture::Type::DIFFUSE);
}

Texture2D::~Texture2D() {
  FreeGPU();
}

void Texture2D::Setup(usize width, usize height, GLenum format, GLenum type,
                      const void* data) {
  _texture.As<gl::TextureTarget::TEXTURE_2D>([&](gl::Texture2DView& self) {
    self.Reserve2D(GL_RGB, width, height, format, type, data);

    self.SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    self.SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

    self.SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    self.SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  });
}

void Texture2D::Bind() const noexcept {
  _texture.As<gl::TextureTarget::TEXTURE_2D>().Bind();
}

void Texture2D::Unbind() const noexcept {
  _texture.As<gl::TextureTarget::TEXTURE_2D>().Unbind();
}

void Texture2D::ToGPU(const void* data, GLenum format, bool unbind) {
  Bind();

  _texture.As<gl::TextureTarget::TEXTURE_2D>().Reserve2D(
      GL_RGB, _width, _height, format, GL_UNSIGNED_BYTE, data);

  if (unbind) {
    Unbind();
  }
}

void Texture2D::FreeGPU() noexcept {
  _texture.As<gl::TextureTarget::TEXTURE_2D>().Clear2D();
}

void Texture2D::SetParameter(GLenum pname, GLint value) {
  _texture.As<gl::TextureTarget::TEXTURE_2D>().SetParameter(pname, value);
}

GLint Texture2D::GetParameter(GLenum pname) {
  return _texture.As<gl::TextureTarget::TEXTURE_2D>().GetParameter(pname);
}

}  // namespace over
