#include <over/core/opengl/Framebuffer.hpp>

#include <cassert>

#include <over/core/Includes.hpp>

namespace over {
Framebuffer::Framebuffer() : _id(0) {
  glGenFramebuffers(1, &_id);
}

Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &_id);
}

void Framebuffer::Bind() noexcept {
  assert(_id != 0);
  glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void Attach(GLenum attachment, Texture2D& texture) {
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.Get(), 0);
}

void Framebuffer::Unbind() noexcept {
  assert(_id != 0);
  // TODO: assert _id is binded
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}  // namespace over
