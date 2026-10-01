#include <owlet/gl/targets/uniform_buffer.h>

#include <glad/gl.h>

#include <owlet/debug/gl.h>
#include <owlet/gl/buffer.h>

namespace owlet::gl {
UniformBuffer& UniformBuffer::AttachUniform(int32 unit) {
    auto gl = m_buffer->GL();
    auto id = static_cast<GLuint>(m_buffer->Id());

    gl->BindBufferBase(s_target, static_cast<GLuint>(unit), id);
    debug::GLCheckError(gl);

    return *this;
}
}    // namespace owlet::gl
