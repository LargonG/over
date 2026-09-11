#include <owlet/gl/vertex_array.h>

#include <glad/gl.h>
#include <owlet/debug/core.h>
#include <owlet/debug/gl.h>
#include <owlet/gl/buffer.h>
#include <owlet/gl/memory.h>
#include <owlet/gl/targets/index_buffer.h>
#include <owlet/types.h>

namespace owlet::gl {
VertexArrayId SimpleVertexArrayAllocator::Alloc(Context* gl) {
    GLuint res = 0;
    gl->CreateVertexArrays(1, &res);
    debug::GLCheckError(gl);

    return VertexArrayId(res);
}

void SimpleVertexArrayAllocator::Dealloc(Context* gl, VertexArrayId id) noexcept {
    GLuint raw_id = static_cast<GLuint>(id);
    debug::Assert(gl->IsVertexArray(raw_id));

    gl->DeleteVertexArrays(1, &raw_id);
    debug::GLCheckError(gl);
}

VertexArray::VertexArray(Context* gl, VertexArrayAllocator* alloc)
    : Object(gl, [](Context* gl) { return gl->DefaultVertexArrayAllocator(); }, alloc) {}

VertexArray& VertexArray::Enable(int32 location) {
    auto* gl = GL();

    gl->EnableVertexArrayAttrib(RawId(), static_cast<GLuint>(location));
    debug::GLCheckError(gl);

    return *this;
}

VertexArray& VertexArray::Disable(int32 location) {
    auto* gl = GL();

    gl->DisableVertexArrayAttrib(RawId(), static_cast<GLuint>(location));
    debug::GLCheckError(gl);

    return *this;
}

VertexArray& VertexArray::Format(int32 location, GLenum type, int32 count, int32 relative_offset) {
    auto* gl = GL();

    gl->VertexArrayAttribFormat(RawId(), static_cast<GLuint>(location), static_cast<GLint>(count), type, GL_FALSE,
                                static_cast<GLuint>(relative_offset));
    debug::GLCheckError(gl);

    return *this;
}

VertexArray& VertexArray::Attach(int32 binding_id, Buffer& buf, int64 offset, int32 shift) {
    auto* gl = GL();
    auto raw_buf_id = static_cast<GLuint>(buf.Id());

    gl->VertexArrayVertexBuffer(RawId(), static_cast<GLuint>(binding_id), raw_buf_id, static_cast<GLintptr>(offset),
                                static_cast<GLsizei>(shift));
    debug::GLCheckError(gl);

    return *this;
}

VertexArray& VertexArray::BindFormat(int32 binding_id, int32 location) {
    auto* gl = GL();

    gl->VertexArrayAttribBinding(RawId(), static_cast<GLuint>(location), static_cast<GLuint>(binding_id));
    debug::GLCheckError(gl);

    return *this;
}

VertexArray& VertexArray::BindFormats(int32 binding_id, std::span<const int32> locations) {
    for (const auto& location : locations) {
        BindFormat(binding_id, location);
    }
    return *this;
}

VertexArray& VertexArray::AttachIndex(IndexBuffer& id_buf) {
    auto* gl = GL();
    auto raw_buf_id = static_cast<GLuint>(id_buf.Owner().Id());

    gl->VertexArrayElementBuffer(RawId(), raw_buf_id);
    debug::GLCheckError(gl);

    return *this;
}

VertexArray& VertexArray::Bind() {
    auto* gl = GL();

    gl->BindVertexArray(RawId());
    debug::GLCheckError(gl);

    return *this;
}

VertexArray& VertexArray::Unbind() {
    auto* gl = GL();

    gl->BindVertexArray(0);    // warning: should set to previous binded id
    debug::GLCheckError(gl);

    return *this;
}

}    // namespace owlet::gl
