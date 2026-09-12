#include <owlet/gl/frame_buffer.h>

#include <utility>

#include <owlet/debug/core.h>
#include <owlet/debug/gl.h>
#include <owlet/gl/memory.h>
#include <owlet/gl/texture.h>

#include <glad/gl.h>

namespace owlet::gl {
RenderBuffer::RenderBuffer(Context* gl, RenderBufferAllocator* alloc)
    : Object(
          gl, [](Context* gl) { return gl->DefaultRenderBufferAllocator(); }, alloc),
      m_size(0, 0),
      m_format(GL_DEPTH24_STENCIL8),
      m_initialized(false) {}

RenderBuffer& RenderBuffer::Alloc(GLenum internal_format, std::tuple<usize, usize> size) {
    debug::Require(!m_initialized, "RenderBuffer has already been initialized, create new one");
    auto* gl = GL();

    auto [size_x, size_y] = size;

    gl->NamedRenderbufferStorage(RawId(), internal_format, static_cast<GLsizei>(size_x), static_cast<GLsizei>(size_y));

    m_format = internal_format;
    m_size = size;
    m_initialized = true;

    return *this;
}

FrameBuffer::FrameBuffer(Context* gl, FrameBufferAllocator* alloc)
    : Object(
          gl, [](Context* gl) { return gl->DefaultFrameBufferAllocator(); }, alloc),
      m_read_buffer(GL_COLOR_ATTACHMENT0),
      m_draw_buffers({GL_COLOR_ATTACHMENT0, GL_DEPTH_STENCIL_ATTACHMENT}) {}

FrameBuffer& FrameBuffer::Attach(GLenum target, RenderBuffer& rb) {
    auto* gl = GL();

    gl->NamedFramebufferRenderbuffer(RawId(), target, GL_RENDERBUFFER, static_cast<GLuint>(rb.Id()));
    debug::GLCheckError(gl);

    return *this;
}

FrameBuffer& FrameBuffer::Attach(GLenum target, int32 level, Texture2D& texture) {
    auto* gl = GL();

    gl->NamedFramebufferTexture(RawId(), target, static_cast<GLuint>(texture.Id()), level);
    debug::GLCheckError(gl);

    return *this;
}

FrameBuffer& FrameBuffer::Attach(GLenum target, Texture2DMultiSample& texture) {
    auto* gl = GL();

    gl->NamedFramebufferTexture(RawId(), target, static_cast<GLuint>(texture.Id()), 0);
    debug::GLCheckError(gl);

    return *this;
}

FrameBuffer& FrameBuffer::DrawBuffers(std::vector<GLenum> draw_buffers) {
    auto* gl = GL();

    m_draw_buffers = std::move(draw_buffers);

    gl->NamedFramebufferDrawBuffers(RawId(), static_cast<GLsizei>(m_draw_buffers.size()), m_draw_buffers.data());
    debug::GLCheckError(gl);

    return *this;
}

FrameBuffer& FrameBuffer::ReadBuffer(GLenum target) {
    auto* gl = GL();

    gl->NamedFramebufferReadBuffer(RawId(), target);
    debug::GLCheckError(gl);

    m_read_buffer = target;

    return *this;
}

bool FrameBuffer::IsValid(GLenum target) {
    auto* gl = GL();

    GLenum result = gl->CheckNamedFramebufferStatus(RawId(), target);
    debug::GLCheckError(gl);

    return result == GL_FRAMEBUFFER_COMPLETE;
}

}    // namespace owlet::gl
