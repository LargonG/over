#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/Binded.hpp>
#include <over/core/opengl/wrappers/RenderBufferWrapper.hpp>

#include <functional>

namespace over {

class RenderBuffer : public Binded<RenderBuffer> {
 public:
  RenderBuffer() noexcept = default;
  RenderBuffer(uint32 width, uint32 height, GLenum internalFormat);

  RenderBuffer(const RenderBuffer&) = delete;
  RenderBuffer& operator=(const RenderBuffer&) = delete;

  RenderBuffer(RenderBuffer&&) noexcept = default;
  RenderBuffer& operator=(RenderBuffer&&) noexcept = default;

  ~RenderBuffer() = default;

  void Bind() const;
  void Unbind() const;

  // Use only for low level functions
  GLuint Get() const noexcept { return *_buffer.Get(); }

 private:
  void Setup(uint32 width, uint32 height, GLenum internalFormat);

  gl::RenderBufferWrapper<> _buffer;
};
}  // namespace over
