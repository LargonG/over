#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/RenderBuffer.hpp>
#include <over/core/opengl/Texture2D.hpp>

namespace over {
class Framebuffer : public Binded<Framebuffer> {
 public:
  Framebuffer() noexcept;

  Framebuffer(const Framebuffer&) = delete;  // лень + не факт что нужно будет
  Framebuffer(Framebuffer&&) = delete;

  Framebuffer& operator=(const Framebuffer&) = delete;
  Framebuffer& operator=(Framebuffer&&) = delete;

  ~Framebuffer();

  void Setup();

  void Bind() const noexcept;
  void Unbind() const noexcept;

  template <class Allocator>
  void Attach(GLenum attachment,
              const gl::TextureWrapper<Allocator>& owner) noexcept {
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D,
                           *owner.Get(), 0);
  }

  void Attach(GLenum attachment, const Texture2D& texture) noexcept;
  void Attach(GLenum attachment, const RenderBuffer& buffer) noexcept;

  bool IsReady();

 private:
  uint32 _id;
};
}  // namespace over
