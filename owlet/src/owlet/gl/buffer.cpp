#include <owlet/gl/buffer.h>

#include <glad/gl.h>

#include <owlet/debug/gl.h>
#include <owlet/gl/context.h>
#include <owlet/types.h>

namespace owlet::gl {

BufferId SimpleBufferAllocator::Alloc(Context* gl) {
    GLuint area;
    gl->CreateBuffers(1, &area);
    debug::GLCheckError(gl);

    return BufferId(area);
}

void SimpleBufferAllocator::Dealloc(Context* gl, BufferId id) noexcept {
    GLuint raw = static_cast<GLuint>(id);
    gl->DeleteBuffers(1, &raw);
    debug::GLCheckError(gl);
}

Buffer::Buffer(Context* context, BufferAllocator* allocator)
    : m_handler((allocator == nullptr ? context->DefaultBufferAllocator() : allocator), context) {}

void Buffer::Write(usize size, anytype data, Usage usage) {
    m_handler.Context()->NamedBufferData(m_handler.GetRaw(), static_cast<GLsizeiptr>(size), data,
                                         static_cast<GLenum>(usage));
    debug::GLCheckError(m_handler.Context());
}

}    // namespace owlet::gl
