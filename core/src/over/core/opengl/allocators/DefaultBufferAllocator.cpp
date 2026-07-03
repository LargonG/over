#include <over/core/opengl/allocators/DefaultBufferAllocator.hpp>

#include <cassert>

#include <over/core/Includes.hpp>
#include <over/core/opengl/wrappers/Exception.hpp>

namespace over::gl {

Address DefaultBufferAllocator::Alloc() {
  GLuint id = 0;
  glthrow(glGenBuffers(1, &id));
  return Address(id);
}

void DefaultBufferAllocator::Dealloc(Address ptr) {
  if (*ptr == 0) {
    return;
  }

  assert(glIsBuffer(*ptr));
  glthrow(glDeleteBuffers(1, &*ptr));
}

}  // namespace over::gl
