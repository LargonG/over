#pragma once

#include <glad/gl.h>
#include <owlet/gl/targets/buffer_target.h>

namespace owlet::gl {
struct IndexBuffer : BufferTarget<IndexBuffer> {
  public:
    WL_DEF_BUFFER_TARGET(GL_ELEMENT_ARRAY_BUFFER);
};
}    // namespace owlet::gl
