#include <over/core/opengl/Framebuffer.hpp>

#include <over/core/Includes.hpp>
#include <over/core/opengl/views/FrameBufferView.hpp>
#include <over/core/opengl/views/RenderBufferView.hpp>

namespace over {

void Framebuffer::Setup() {
  volatile auto _ = _frame.As<gl::FrameBufferTarget::FRAMEBUFFER>();
}

void Framebuffer::Bind() const noexcept {
  _frame.As<gl::FrameBufferTarget::FRAMEBUFFER>().Bind();
}

void Framebuffer::Unbind() const noexcept {
  _frame.As<gl::FrameBufferTarget::FRAMEBUFFER>().Unbind();
}

void Framebuffer::Attach(GLenum attachment, const Texture2D& texture) noexcept {
  _frame.As<gl::FrameBufferTarget::FRAMEBUFFER>().Attach(
      attachment, gl::TextureView<gl::TextureTarget::TEXTURE_2D>(texture.Get()),
      0);
}

void Framebuffer::Attach(GLenum attachment,
                         const RenderBuffer& buffer) noexcept {
  _frame.As<gl::FrameBufferTarget::FRAMEBUFFER>().Attach(
      attachment, gl::RenderBufferView<gl::RenderBufferTarget::RENDER_BUFFER>(
                      buffer.Get()));
}

bool Framebuffer::IsReady() {
  return _frame.As<gl::FrameBufferTarget::FRAMEBUFFER>().IsReady();
}

}  // namespace over
