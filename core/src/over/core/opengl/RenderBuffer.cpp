#include <over/core/opengl/RenderBuffer.hpp>

#include <cassert>
#include <exception>
#include <over/core/opengl/views/RenderBufferView.hpp>

namespace over {

RenderBuffer::RenderBuffer(uint32 width, uint32 height, GLenum internalFormat)
    : RenderBuffer() {
  Setup(width, height, internalFormat);
}

void RenderBuffer::Bind() const {
  _buffer.As<gl::RenderBufferTarget::RENDER_BUFFER>().Bind();
}

void RenderBuffer::Unbind() const {
  _buffer.As<gl::RenderBufferTarget::RENDER_BUFFER>().Unbind();
}

void RenderBuffer::Setup(uint32 width, uint32 height, GLenum internalFormat) {
  _buffer.As<gl::RenderBufferTarget::RENDER_BUFFER>(
      [&](gl::RenderBufferView<gl::RenderBufferTarget::RENDER_BUFFER>& self) {
        self.Reserve(internalFormat, width, height);
      });
}

}  // namespace over
