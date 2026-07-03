#pragma once

#include <over/core/Includes.hpp>
#include <over/core/opengl/Address.hpp>

namespace over::gl {
class DefaultRenderBufferAllocator {
 public:
  DefaultRenderBufferAllocator() = default;

  [[nodiscard]] Address Alloc();
  void Dealloc(Address ptr);

 private:
  // stateless allocator
};
}  // namespace over::gl
