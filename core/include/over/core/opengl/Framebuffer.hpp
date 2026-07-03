#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/RenderBuffer.hpp>
#include <over/core/opengl/Texture2D.hpp>
#include <over/core/opengl/targets/FrameBufferTarget.hpp>
#include <over/core/opengl/views/FrameBufferView.hpp>
#include <over/core/opengl/wrappers/FrameBufferWrapper.hpp>

namespace over {
class Framebuffer : public Binded<Framebuffer> {
 public:
  Framebuffer() noexcept = default;

  Framebuffer(const Framebuffer&) = delete;  // лень + не факт что нужно будет
  Framebuffer& operator=(const Framebuffer&) = delete;

  Framebuffer(Framebuffer&&) noexcept = default;
  Framebuffer& operator=(Framebuffer&&) noexcept = default;

  ~Framebuffer() = default;

  void Setup();

  void Bind() const noexcept;
  void Unbind() const noexcept;

  template <class Allocator>
  void Attach(GLenum attachment,
              const gl::TextureWrapper<Allocator>& owner) noexcept {
    _frame.As<gl::FrameBufferTarget::FRAMEBUFFER>().Attach(
        attachment, owner.As<gl::TextureTarget::TEXTURE_2D>(), 0);
  }

  void Attach(GLenum attachment, const Texture2D& texture) noexcept;
  void Attach(GLenum attachment, const RenderBuffer& buffer) noexcept;

  bool IsReady();

 private:
  gl::FrameBufferWrapper<> _frame;
};
}  // namespace over
