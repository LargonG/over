#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/targets/FrameBufferTarget.hpp>
#include <over/core/opengl/views/FrameBufferView.hpp>
#include <over/core/opengl/wrappers/FrameBufferWrapper.hpp>

namespace over::gl {
class FrameBuffer final {
 public:
  struct Box {
    usize left;
    usize down;
    usize right;
    usize up;
  };

  template <class Al1, class Al2>
  static void Copy(FrameBufferWrapper<Al1>& from, FrameBufferWrapper<Al2>& to,
                   Box a, Box b, GLbitfield mask, GLenum filter) {
    auto readFrame = from.As<gl::FrameBufferTarget::READ_FRAMEBUFFER>();
    auto writeFrame = to.As<gl::FrameBufferTarget::DRAW_FRAMEBUFFER>();

    readFrame.Use([&] {
      writeFrame.Use([&] {
        glthrow(glBlitFramebuffer(a.left, a.down, a.right, a.up, b.left, b.down,
                                  b.right, b.up, mask, filter));
      });
    });
  }
};
}  // namespace over::gl
