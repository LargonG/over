#include <owlet/gl/frame_buffer.h>

#include <owlet/debug/core.h>
#include <owlet/debug/gl.h>
#include <owlet/gl/texture.h>

#include <glad/gl.h>
#include <utility>

namespace owlet::gl {
RenderBuffer::RenderBuffer(Context* gl, RenderBufferAllocator* alloc)
    : m_handler(alloc ? alloc : gl->DefaultRenderBufferAllocator(), gl),
      m_size(0, 0),
      m_format(GL_DEPTH24_STENCIL8),
      m_initialized(false) {}

RenderBuffer& RenderBuffer::Alloc(GLenum internal_format, std::tuple<usize, usize> size) {
    debug::Require(!m_initialized, "RenderBuffer has already been initialized, create new one");
    auto* gl = m_handler.Context();

    auto [size_x, size_y] = size;

    gl->NamedRenderbufferStorage(m_handler.GetRaw(), internal_format, static_cast<GLsizei>(size_x),
                                 static_cast<GLsizei>(size_y));

    m_format = internal_format;
    m_size = size;
    m_initialized = true;

    return *this;
}

FrameBuffer::FrameBuffer(Context* gl, FrameBufferAllocator* alloc)
    : m_handler(alloc ? alloc : gl->DefaultFrameBufferAllocator(), gl),
      m_read_buffer(GL_COLOR_ATTACHMENT0),
      m_draw_buffers({GL_COLOR_ATTACHMENT0, GL_DEPTH_STENCIL_ATTACHMENT}) {}

FrameBuffer& FrameBuffer::Attach(GLenum target, RenderBuffer& rb) {
    auto* gl = m_handler.Context();

    gl->NamedFramebufferRenderbuffer(m_handler.GetRaw(), target, GL_RENDERBUFFER, rb.m_handler.GetRaw());
    debug::GLCheckError(gl);

    return *this;
}

FrameBuffer& FrameBuffer::Attach(GLenum target, int32 level, Texture2D& texture) {
    auto* gl = m_handler.Context();

    gl->NamedFramebufferTexture(m_handler.GetRaw(), target, texture.m_handler.GetRaw(), level);
    debug::GLCheckError(gl);

    return *this;
}

FrameBuffer& FrameBuffer::Attach(GLenum target, Texture2DMultiSample& texture) {
    auto* gl = m_handler.Context();

    gl->NamedFramebufferTexture(m_handler.GetRaw(), target, texture.m_handler.GetRaw(), 0);
    debug::GLCheckError(gl);

    return *this;
}

FrameBuffer& FrameBuffer::DrawBuffers(std::vector<GLenum> draw_buffers) {
    auto* gl = m_handler.Context();

    m_draw_buffers = std::move(draw_buffers);

    gl->NamedFramebufferDrawBuffers(m_handler.GetRaw(), static_cast<GLsizei>(m_draw_buffers.size()),
                                    m_draw_buffers.data());
    debug::GLCheckError(gl);

    return *this;
}

FrameBuffer& FrameBuffer::ReadBuffer(GLenum target) {
    auto* gl = m_handler.Context();

    gl->NamedFramebufferReadBuffer(m_handler.GetRaw(), target);
    debug::GLCheckError(gl);

    m_read_buffer = target;

    return *this;
}

bool FrameBuffer::IsValid(GLenum target) {
    auto* gl = m_handler.Context();

    GLenum result = gl->CheckNamedFramebufferStatus(m_handler.GetRaw(), target);
    debug::GLCheckError(gl);

    return result == GL_FRAMEBUFFER_COMPLETE;
}

}    // namespace owlet::gl
