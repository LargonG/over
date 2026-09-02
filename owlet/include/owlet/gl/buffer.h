#pragma once

#include <glad/gl.h>

#include <owlet/debug/gl.h>
#include <owlet/gl/memory.h>

namespace owlet::gl {

struct SimpleBufferAllocator : BufferAllocator {
    SimpleBufferAllocator() {}

    [[nodiscard]] BufferId Alloc(Context*) override;

    void Dealloc(Context*, BufferId) noexcept override;
};

template <class T>
struct BufferTarget;

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

    explicit Buffer(Context* context, BufferAllocator* allocator = nullptr);

    Buffer& Alloc(usize size, const void* data, Usage usage);

    Buffer& Write(usize offset, usize size, const void* data);

    template <class T, typename F>
        requires std::is_invocable_v<F, T*, usize>
    Buffer& Map(Access access, F&& func) {
        auto* gl = m_handler.Context();
        auto id = m_handler.GetRaw();

        T* value = reinterpret_cast<T*>(gl->MapNamedBuffer(id, static_cast<GLenum>(access)));
        debug::GLCheckError(gl);

        std::forward<F>(func)(value, Size<T>());

        gl->UnmapNamedBuffer(id);
        debug::GLCheckError(gl);

        return *this;
    }

    template <class T, typename F>
    T As(F&& func) {
        auto val = T(*this);
        std::forward<F>(func)(val);
        return val;
    }

    template <class T>
    T As() {
        return T(*this);
    }

    template <class T>
    usize Size() {
        return m_size_in_bytes / sizeof(T);
    }

  private:
    Handler<BufferId> m_handler;
    usize m_size_in_bytes;

    template <class T>
    friend struct ::owlet::gl::BufferTarget;
};
}    // namespace owlet::gl
