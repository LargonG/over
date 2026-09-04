#include <owlet/gl/targets/uniform_buffer.h>

#include <glad/gl.h>

#include <owlet/debug/gl.h>
#include <owlet/gl/buffer.h>

namespace owlet::gl {
UniformBuffer& UniformBuffer::AttachUniform(int32 unit) {
    auto gl = GL();

    gl->BindBufferBase(s_target, static_cast<GLuint>(unit), Id());
    debug::GLCheckError(gl);

    return *this;
}
}    // namespace owlet::gl
