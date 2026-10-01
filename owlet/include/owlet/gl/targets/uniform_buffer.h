#pragma once

#include <glad/gl.h>
#include <owlet/gl/targets/buffer_target.h>

namespace owlet::gl {
struct UniformBuffer : BufferTarget<UniformBuffer> {
  public:
    WL_DEF_BUFFER_TARGET(GL_UNIFORM_BUFFER);

    UniformBuffer& AttachUniform(int32 unit);
};
}    // namespace owlet::gl
