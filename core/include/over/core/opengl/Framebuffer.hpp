#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/Texture2D.hpp>

namespace over {
class Framebuffer {
 public:
  Framebuffer();

  Framebuffer(const Framebuffer&) = delete;  // лень + не факт что нужно будет
  Framebuffer(Framebuffer&&) = delete;

  Framebuffer& operator=(const Framebuffer&) = delete;
  Framebuffer& operator=(Framebuffer&&) = delete;

  ~Framebuffer();

  void Bind() noexcept;

  void Attach(GLenum attachment, Texture2D& texture) noexcept;

  void Unbind() noexcept;

 private:
  uint32 _id;
};
}  // namespace over
