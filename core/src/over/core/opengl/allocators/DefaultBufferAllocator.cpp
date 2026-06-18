#include <over/core/opengl/allocators/DefaultBufferAllocator.hpp>

namespace over {

DefaultBufferAllocator::DefaultBufferAllocator() {}

DefaultBufferAllocator::~DefaultBufferAllocator() {}

GLuint DefaultBufferAllocator::Alloc() {
  GLuint id = 0;
  glGenBuffers(1, &id);
  // TODO: throw exception if soumething went wrong
  return id;
}

void DefaultBufferAllocator::Dealloc(GLuint id) noexcept {
  glDeleteBuffers(1, &id);
}

}  // namespace over
