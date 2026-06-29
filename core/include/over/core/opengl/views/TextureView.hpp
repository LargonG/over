#pragma once

#include <over/core/opengl/targets/TextureTarget.hpp>
#include <over/core/opengl/wrappers/Exception.hpp>
#include <over/core/opengl/wrappers/TextureWrapper.hpp>

namespace over::gl {

template <TextureTarget Target>
class TextureView : public Binded<TextureView<Target>> {
  constexpr static GLenum _target = static_cast<GLenum>(Target);

 public:
  TextureView() : _ptr(nullptr) {}
  TextureView(nullptr_t) : _ptr(nullptr) {}

  TextureView(const TextureView&) = default;
  TextureView& operator=(const TextureView&) = default;

  TextureView(TextureView&&) noexcept = default;
  TextureView& operator=(TextureView&&) noexcept = default;

  ~TextureView() = default;

  template <class Allocator>
  explicit TextureView(const TextureWrapper<Allocator>* me) : _ptr(me->Get()) {}

  void Bind() const { glthrow(glBindTexture(_target, *_ptr)); }

  void Unbind() const { glthrow(glBindTexture(_target, 0)); }

  void Reserve2D(int32 internalFormat, usize width, usize height, GLenum format,
                 GLenum type, const void* data) {
    Reserve2DAs(_target, internalFormat, width, height, format, type, data);
  }

  void Reserve2DAs(GLenum target, int32 internalFormat, usize width,
                   usize height, GLenum format, GLenum type, const void* data) {
    glthrow(glTexImage2D(target, 0, static_cast<GLint>(internalFormat),
                         static_cast<GLsizei>(width),
                         static_cast<GLsizei>(height), 0, format, type, data));
  }

  void Clear2D(GLenum internalFormat = GL_RGB, GLenum format = GL_RGB,
               GLenum type = GL_UNSIGNED_BYTE) {
    glthrow(glTexImage2D(_target, 0, internalFormat, 0, 0, 0, format, type,
                         nullptr));
  }

  void GenerateMipmap() { glthrow(glGenerateMipmap(_target)); }

  [[nodiscard]] GLint GetParameter(GLenum name) const {
    GLint result = 0;
    glthrow(glGetTexParameteriv(_target, name, &result));
    return result;
  }

  void SetParameter(GLenum name, GLint value) {
    glthrow(glTexParameteri(_target, name, value));
  }

 private:
  Address _ptr;
};

using Texture2DView = TextureView<TextureTarget::TEXTURE_2D>;
using CubeMapView = TextureView<TextureTarget::TEXTURE_CUBE_MAP>;
}  // namespace over::gl
