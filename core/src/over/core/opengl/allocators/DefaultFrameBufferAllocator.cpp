#include <over/core/opengl/allocators/DefaultFrameBufferAllocator.hpp>

#include <over/core/Includes.hpp>
#include <over/core/opengl/wrappers/Exception.hpp>

namespace over::gl {
Address DefaultFrameBufferAllocator::Alloc() {
  GLuint id = 0;
  glthrow(glGenFramebuffers(1, &id));
  return Address(id);
}

void DefaultFrameBufferAllocator::Dealloc(Address ptr) {
  if (0 == *ptr) {
    return;
  }

  assert(glIsFramebuffer(*ptr));
  glthrow(glDeleteFramebuffers(1, &*ptr));
}

}  // namespace over::gl
