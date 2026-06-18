#pragma once

#include <over/core/Includes.hpp>

namespace over {

class DefaultBufferAllocator {
 public:
  DefaultBufferAllocator();

  ~DefaultBufferAllocator();

  [[nodiscard]] GLuint Alloc();
  void Dealloc(GLuint id) noexcept;

 private:
  // stateless allocator
};
}  // namespace over
