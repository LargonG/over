#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>

namespace over::gl {
class Texture final {
 public:
  static void Activate(GLenum target);
  static GLenum GetFormat(usize channels);
};
}  // namespace over::gl
