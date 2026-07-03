#include <over/core/opengl/allocators/DefaultTextureAllocator.hpp>

#include <cassert>

#include <over/core/Includes.hpp>
#include <over/core/opengl/wrappers/Exception.hpp>

namespace over::gl {
Address DefaultTextureAllocator::Alloc() {
  GLuint result = 0;
  glthrow(glGenTextures(1, &result));
  return Address(result);
}

void DefaultTextureAllocator::Dealloc(Address ptr) {
  if (*ptr == 0) {
    return;
  }

  assert(glIsTexture(*ptr));
  glthrow(glDeleteTextures(1, &*ptr));
}
}  // namespace over::gl
