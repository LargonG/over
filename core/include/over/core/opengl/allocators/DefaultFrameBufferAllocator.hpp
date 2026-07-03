#pragma once

#include <over/core/opengl/Address.hpp>

namespace over::gl {
class DefaultFrameBufferAllocator {
 public:
  DefaultFrameBufferAllocator() = default;

  [[nodiscard]] Address Alloc();
  void Dealloc(Address ptr);

 private:
  // stateless
};
}  // namespace over::gl
