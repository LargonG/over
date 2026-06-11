#include <over/core/opengl/RenderBuffer.hpp>

#include <cassert>
#include <exception>

namespace over {
RenderBuffer::RenderBuffer() noexcept : _id(0) {}

RenderBuffer::RenderBuffer(uint32 width, uint32 height, GLenum internalFormat) {
  Setup(width, height, internalFormat);
}

RenderBuffer& RenderBuffer::operator=(RenderBuffer&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  FreeGPU();
  _id = std::exchange(other._id, 0);

  return *this;
}

RenderBuffer::~RenderBuffer() {
  if (0 != _id) {
    glDeleteRenderbuffers(1, &_id);
  }
}

void RenderBuffer::Bind() const noexcept {
  assert(0 != _id);

  glBindRenderbuffer(GL_RENDERBUFFER, _id);
}

void RenderBuffer::Unbind() const noexcept {
  assert(0 != _id);

  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBuffer::Setup(uint32 width, uint32 height, GLenum internalFormat) {
  assert(0 != _id);

  glGenRenderbuffers(1, &_id);

  Use([=]() {
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat,
                          static_cast<GLsizei>(width),
                          static_cast<GLsizei>(height));
    auto err = glGetError();
    if (err == GL_OUT_OF_MEMORY) {
      throw std::
          bad_alloc();  // TODO: make OpenGL exceptions, maybe... Or don't throw them
    }
  });
}

void RenderBuffer::FreeGPU() noexcept {
  if (0 == _id) {
    return;
  }

  glDeleteRenderbuffers(1, &_id);
  _id = 0;
}

}  // namespace over
