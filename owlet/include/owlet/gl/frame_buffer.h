#pragma once

#include <tuple>

#include <owlet/gl/memory.h>

#include <owlet/types.h>
#include <vector>

namespace owlet::gl {

struct Texture2D;
struct Texture2DMultiSample;

struct SimpleFrameBufferAllocator : FrameBufferAllocator {};

struct SimpleRenderBufferAllocator : RenderBufferAllocator {};

struct RenderBuffer : Object<RenderBufferId> {
  public:
    RenderBuffer(Context*, RenderBufferAllocator* = nullptr);

    RenderBuffer& Alloc(GLenum internal_format, std::tuple<usize, usize> size);

  private:
    GLenum m_format;
    std::tuple<usize, usize> m_size;
    bool m_initialized;
};

struct FrameBuffer : Object<FrameBufferId> {
  public:
    FrameBuffer(Context*, FrameBufferAllocator* = nullptr);

    FrameBuffer& Attach(GLenum target, RenderBuffer&);
    FrameBuffer& Attach(GLenum target, int32 level, Texture2D&);
    FrameBuffer& Attach(GLenum target, Texture2DMultiSample&);

    FrameBuffer& DrawBuffers(std::vector<GLenum> draw_buffers);
    FrameBuffer& ReadBuffer(GLenum target);

    [[nodiscard]] bool IsValid(GLenum target);

  private:
    GLenum m_read_buffer;
    std::vector<GLenum> m_draw_buffers;
};
}    // namespace owlet::gl
