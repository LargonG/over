#include <over/core/opengl/allocators/DefaultLayoutAllocator.hpp>

#include <over/core/Includes.hpp>

namespace over::gl {
Address DefaultLayoutAllocator::Alloc() {
  GLuint id = 0;
  glGenVertexArrays(1, &id);
  return Address(id);
}

void DefaultLayoutAllocator::Dealloc(Address ptr) {
  if (0 == *ptr) {
    return;
  }

  assert(glIsVertexArray(*ptr));
  glDeleteVertexArrays(1, &*ptr);
}
}  // namespace over::gl
