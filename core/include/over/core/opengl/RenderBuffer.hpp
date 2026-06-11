#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/Binded.hpp>

#include <functional>

namespace over {

class RenderBuffer : public Binded<RenderBuffer> {
 public:
  RenderBuffer(uint32 width, uint32 height, GLenum internalFormat);

  RenderBuffer() noexcept;

  RenderBuffer(const RenderBuffer&) = delete;
  RenderBuffer(RenderBuffer&&) noexcept = default;

  RenderBuffer& operator=(const RenderBuffer&) = delete;
  RenderBuffer& operator=(RenderBuffer&&) noexcept;

  ~RenderBuffer();

  void Bind() const noexcept;
  void Unbind() const noexcept;

  void FreeGPU() noexcept;

  // Use only for low level functions
  GLuint Get() const noexcept { return _id; }

 private:
  void Setup(uint32 width, uint32 height, GLenum internalFormat);

  GLuint _id;
};
}  // namespace over
