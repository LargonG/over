#pragma once

#include <over/core/opengl/Address.hpp>

namespace over::gl {
class DefaultTextureAllocator {
 public:
  DefaultTextureAllocator() = default;

  [[nodiscard]] Address Alloc();
  void Dealloc(Address ptr);

 private:
  // stateless allocator
};
}  // namespace over::gl
