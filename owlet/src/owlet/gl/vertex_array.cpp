#include <owlet/gl/vertex_array.h>

#include <glad/gl.h>
#include <owlet/debug/gl.h>
#include <owlet/gl/buffer.h>
#include <owlet/types.h>

namespace owlet::gl {
VertexArray::VertexArray(Context* gl, VertexArrayAllocator* alloc)
    : m_handler(alloc ? alloc : gl->DefaultVertexArrayAllocator(), gl) {}

VertexArray& VertexArray::Enable(int32 location) {
    auto* gl = m_handler.Context();

    gl->EnableVertexArrayAttrib(m_handler.GetRaw(), static_cast<GLuint>(location));
    debug::GLCheckError(gl);

    return *this;
}

VertexArray& VertexArray::Disable(int32 location) {
    auto* gl = m_handler.Context();

    gl->DisableVertexArrayAttrib(m_handler.GetRaw(), static_cast<GLuint>(location));
    debug::GLCheckError(gl);

    return *this;
}

VertexArray& VertexArray::Format(int32 location, GLenum type, int32 count, int32 relative_offset) {
    auto* gl = m_handler.Context();

    gl->VertexArrayAttribFormat(m_handler.GetRaw(), static_cast<GLuint>(location), static_cast<GLint>(count), type,
                                GL_FALSE, static_cast<GLuint>(relative_offset));
    debug::GLCheckError(gl);

    return *this;
}

VertexArray& VertexArray::Attach(int32 binding_id, Buffer* buf, int64 offset, int32 shift) {
    auto* gl = m_handler.Context();

    gl->VertexArrayVertexBuffer(m_handler.GetRaw(), static_cast<GLuint>(binding_id), buf->m_handler.GetRaw(),
                                static_cast<GLintptr>(offset), static_cast<GLsizei>(shift));
    debug::GLCheckError(gl);

    return *this;
}

VertexArray& VertexArray::BindFormat(int32 location, int32 binding_id) {
    auto* gl = m_handler.Context();

    gl->VertexArrayAttribBinding(m_handler.GetRaw(), static_cast<GLuint>(location), static_cast<GLuint>(binding_id));
    debug::GLCheckError(gl);

    return *this;
}

VertexArray& VertexArray::Bind() {
    auto* gl = m_handler.Context();

    gl->BindVertexArray(m_handler.GetRaw());
    debug::GLCheckError(gl);

    return *this;
}

VertexArray& VertexArray::Unbind() {
    auto* gl = m_handler.Context();

    gl->BindVertexArray(0);    // warning: should set to previous binded id
    debug::GLCheckError(gl);

    return *this;
}

}    // namespace owlet::gl
