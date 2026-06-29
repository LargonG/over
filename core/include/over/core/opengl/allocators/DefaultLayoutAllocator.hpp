#pragma once

#include <over/core/Types.hpp>
#include <over/core/opengl/Address.hpp>

namespace over::gl {
class DefaultLayoutAllocator {
 public:
  DefaultLayoutAllocator() = default;

  [[nodiscard]] Address Alloc();
  void Dealloc(Address ptr);
};
}  // namespace over::gl
