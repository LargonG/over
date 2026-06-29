#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/Binded.hpp>
#include <over/core/opengl/targets/TextureTarget.hpp>
#include <over/core/opengl/views/TextureView.hpp>
#include <over/core/opengl/wrappers/TextureWrapper.hpp>

namespace over {

class MeshTexture;

class Texture2D : public Binded<Texture2D> {
 public:
  Texture2D() noexcept;
  Texture2D(usize width, usize height, GLenum format, GLenum type,
            const void* data);

  Texture2D(const Texture2D&) = default;
  Texture2D(Texture2D&&) noexcept = default;

  Texture2D& operator=(const Texture2D&);
  Texture2D& operator=(Texture2D&&) noexcept = default;

  explicit operator MeshTexture() const;

  ~Texture2D();

  void ToGPU(const void* data, GLenum format, bool unbind = false);
  void FreeGPU() noexcept;

  void Bind() const noexcept;
  void Unbind() const noexcept;

  void SetParameter(GLenum pname, GLint value);
  GLint GetParameter(GLenum pname);

  const gl::Address& Get() const noexcept { return _texture.Get(); }

 private:
  void Setup(usize width, usize height, GLenum format, GLenum type,
             const void* data);

  usize _width;
  usize _height;
  gl::TextureWrapper<> _texture;
  gl::TextureView<gl::TextureTarget::TEXTURE_2D> _view;
};
}  // namespace over
