#include <over/core/opengl/allocators/DefaultRenderBufferAllocator.hpp>

#include <cassert>

#include <over/core/opengl/wrappers/Exception.hpp>

namespace over::gl {
Address DefaultRenderBufferAllocator::Alloc() {
  GLuint id = 0;
  glthrow(glGenRenderbuffers(1, &id));
  return Address(id);
}

void DefaultRenderBufferAllocator::Dealloc(Address ptr) {
  if (*ptr == 0) {
    return;
  }

  assert(glIsRenderbuffer(*ptr));
  glthrow(glDeleteRenderbuffers(1, &*ptr));
}
}  // namespace over::gl
