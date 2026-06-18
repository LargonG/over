#include <over/core/opengl/Framebuffer.hpp>

#include <cassert>

#include <over/core/Includes.hpp>

#include <fmt/core.h>

namespace over {
Framebuffer::Framebuffer() noexcept : _id(0) {}

Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &_id);
}

void Framebuffer::Setup() {
  if (0 != _id) {
    return;
  }
  glGenFramebuffers(1, &_id);
}

void Framebuffer::Bind() const noexcept {
  glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void Framebuffer::Unbind() const noexcept {
  GLint id = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &id);
  assert(id == _id);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Attach(GLenum attachment, Texture2D& texture) noexcept {
  assert(0 != _id);
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D,
                         texture.Get(), 0);
}

void Framebuffer::Attach(GLenum attachment, RenderBuffer& buffer) noexcept {
  assert(0 != _id);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER,
                            buffer.Get());
}

bool Framebuffer::IsReady() {
  // TODO: assert _id is binded
  return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

}  // namespace over
