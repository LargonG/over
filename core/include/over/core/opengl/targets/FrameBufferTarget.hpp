#pragma once

#include <over/core/Includes.hpp>

namespace over::gl {
enum class FrameBufferTarget : GLenum {
  FRAMEBUFFER = GL_FRAMEBUFFER,
  DRAW_FRAMEBUFFER = GL_DRAW_FRAMEBUFFER,
  READ_FRAMEBUFFER = GL_READ_FRAMEBUFFER
};
}
