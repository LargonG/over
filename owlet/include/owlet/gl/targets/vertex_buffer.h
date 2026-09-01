#pragma once

#include <glad/gl.h>

#include <owlet/gl/targets/buffer_target.h>

namespace owlet::gl {
struct VertexBuffer : BufferTarget<VertexBuffer> {
  public:
    WL_DEF_BUFFER_TARGET(GL_ARRAY_BUFFER);

  private:
};
}    // namespace owlet::gl
