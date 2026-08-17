#pragma once
#pragma once

#include <glad/gl.h>

#include <owlet/debug/gl.h>
#include <owlet/gl/allocator.h>
#include <owlet/gl/context.h>
#include <owlet/gl/handler.h>

namespace owlet::gl {

struct Context;

enum class BufferId : GLuint { Null = 0 };

struct BufferAllocator : Allocator<BufferId> {};

struct SimpleBufferAllocator : BufferAllocator {
    SimpleBufferAllocator() {}

    [[nodiscard]] BufferId Alloc(Context*) override;

    void Dealloc(Context*, BufferId) noexcept override;
};

struct Buffer {
  public:
    enum class Usage : GLenum {
        StaticDraw = GL_STATIC_DRAW,
        DynamicDraw = GL_DYNAMIC_DRAW,
        StreamDraw = GL_STREAM_DRAW,

        StaticCopy = GL_STATIC_COPY,
        DynamicCopy = GL_DYNAMIC_COPY,
        StreamCopy = GL_STREAM_COPY,

        StaticRead = GL_STATIC_READ,
        DynamicRead = GL_DYNAMIC_READ,
        StreamRead = GL_STREAM_READ,
    };

    enum class Access : GLenum {
        Read = GL_READ_ONLY,
        Write = GL_WRITE_ONLY,
        All = GL_READ_WRITE,
    };

    Buffer(Context* context, BufferAllocator* allocator = nullptr);

    void Write(usize size, anytype data, Usage usage);

    template <class T, typename F>
        requires std::is_invocable_v<F, T*>
    void Map(Access access, F&& func) {
        auto* gl = m_handler.Context();
        auto id = m_handler.GetRaw();

        T* value = reinterpret_cast<T*>(gl->MapNamedBuffer(id, static_cast<GLenum>(access)));
        debug::GLCheckError(gl);

        std::forward<F>(func)(value);

        gl->UnmapNamedBuffer(id);
        debug::GLCheckError(gl);
    }

  private:
    Handler<BufferId> m_handler;
};
}    // namespace owlet::gl
