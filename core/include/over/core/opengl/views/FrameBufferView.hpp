#pragma once

#include <over/core/Includes.hpp>
#include <over/core/opengl/Address.hpp>
#include <over/core/opengl/Binded.hpp>
#include <over/core/opengl/targets/RenderBufferTarget.hpp>
#include <over/core/opengl/targets/TextureTarget.hpp>
#include <over/core/opengl/views/RenderBufferView.hpp>
#include <over/core/opengl/wrappers/Exception.hpp>
#include <over/core/opengl/wrappers/FrameBufferWrapper.hpp>

namespace over::gl {
template <FrameBufferTarget Target>
class FrameBufferView : public Binded<FrameBufferView<Target>> {
  constexpr static GLenum _target = static_cast<GLenum>(Target);

 public:
  FrameBufferView() noexcept : _ptr(nullptr) {}
  FrameBufferView(nullptr_t) noexcept : FrameBufferView() {}
  explicit FrameBufferView(Address ptr) noexcept : _ptr(ptr) {}

  FrameBufferView(const FrameBufferView&) = default;
  FrameBufferView& operator=(const FrameBufferView&) = default;

  FrameBufferView(FrameBufferView&&) noexcept = default;
  FrameBufferView& operator=(FrameBufferView&&) noexcept = default;

  ~FrameBufferView() = default;

  void Bind() const { glthrow(glBindFramebuffer(_target, *_ptr)); }

  void Unbind() const { glthrow(glBindFramebuffer(_target, 0)); }

  template <TextureTarget TexTarget>
  void Attach(GLenum attachment, gl::TextureView<TexTarget> view, usize level) {
    constexpr GLenum _textureTarget = static_cast<GLenum>(TexTarget);
    glthrow(glFramebufferTexture2D(_target, attachment, _textureTarget,
                                   *view._ptr, static_cast<GLint>(level)));
  }

  template <RenderBufferTarget RenTarget>
  void Attach(GLenum attachment, gl::RenderBufferView<RenTarget> view) {
    constexpr GLenum _renderBufferTarget = static_cast<GLenum>(RenTarget);
    glthrow(glFramebufferRenderbuffer(_target, attachment, _renderBufferTarget,
                                      *view._ptr));
  }

  bool IsReady() const noexcept {
    return GetStatus() == GL_FRAMEBUFFER_COMPLETE;
  }

  GLenum GetStatus() const noexcept {
    return glthrow(glCheckFramebufferStatus(_target));
  }

  void ReadyOrThrow(std::string_view msg) const {
    if (IsReady()) {
      return;
    }

    throw std::runtime_error(msg.data());
  }

 private:
  Address _ptr;
};
}  // namespace over::gl
