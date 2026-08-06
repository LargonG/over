module;

#include <any>
#include <exception>
#include <type_traits>
#include <utility>

#include <glad/gl.h>

#include <fmt/core.h>

export module owlet.gl:buffer;

import :context;
import :allocator;
import owlet.types;
import owlet.debug;
import :handler;

namespace lowl::gl {

export class BufferAllocator;

export class Buffer final {
  public:
    enum class Id : GLuint { Null = 0 };
    enum class Access : GLenum { Read = GL_READ_ONLY, Write = GL_WRITE_ONLY, All = GL_READ_WRITE };

    enum class Usage : GLenum {
        StreamDraw = GL_STREAM_DRAW,
        StaticDraw = GL_STATIC_DRAW,
        DynamicDraw = GL_DYNAMIC_DRAW,

        StreamRead = GL_STREAM_READ,
        StaticRead = GL_STATIC_READ,
        DynamicRead = GL_DYNAMIC_READ,

        StreamCopy = GL_STREAM_COPY,
        StaticCopy = GL_STATIC_COPY,
        DynamicCopy = GL_DYNAMIC_COPY
    };

  public:
    explicit Buffer(Context&) noexcept;

    Buffer(Context& ctx, BufferAllocator& allocator) noexcept;

    /// <summary>
    /// Allocates memory on GPU and propagates data values
    /// </summary>
    /// <typeparam name="T">Type of data</typeparam>
    /// <param name="count">COUNT, NOT BYTES! Count of elements of type T</param>
    /// <param name="data">C style array, pointer to data</param>
    /// <param name="usage">GPU Usage scenario</param>
    /// <returns></returns>
    template <class T>
    auto Data(usize count, T* data, Usage usage) -> void;

    template <class T, typename F>
        requires std::is_invocable_v<F, T*>
    auto Map(Access, F&& func) noexcept(std::is_nothrow_invocable_v<F, T*>) -> decltype(auto);

    template <class T>
    auto Size() const noexcept {
        return m_size / sizeof(T);
    }
    auto Bytes() const noexcept { return m_size; }

    auto Used() const noexcept { return m_usage; }

  private:
    Handler<Id, BufferAllocator> m_handler;
    GladGLContext* m_gl;

    usize m_size;
    Usage m_usage;
};

export class BufferAllocator : protected GLAllocator<Buffer::Id> {
  public:
    explicit BufferAllocator(Context&);

    // Inherited via GLAllocator
    virtual auto New() noexcept(!lowl::Debug) -> Buffer::Id override;
    virtual auto Delete(Buffer::Id) noexcept(!lowl::Debug) -> void override;

  private:
    GladGLContext* m_gl;

    // For future:
    // This code (below) depends on invariant, that we have only one context in our application, but it's not true
    // So these logic should be implemented in Context class
    // static Allocator* s_default_allocator;
};

}    // namespace lowl::gl

#pragma region Implementation
namespace lowl::gl {

#pragma region Buffer implementation

Buffer::Buffer(Context& ctx) noexcept : Buffer(ctx, ctx.DefaultBufferAllocator()) {}

Buffer::Buffer(Context& ctx, BufferAllocator& alloc) noexcept
    : m_handler(alloc), m_gl(ctx.Get()), m_size(0), m_usage(Usage::StaticDraw) {}

template <class T>
auto Buffer::Data(usize count, T* data, Usage usage) -> void {
    auto raw = m_handler.GetRaw();
    AssertGL<>(m_gl);

    m_size = count * sizeof(T);
    m_usage = usage;

    m_gl->NamedBufferData(raw, static_cast<GLsizeiptr>(m_size), reinterpret_cast<void*>(data),
                          static_cast<GLenum>(m_usage));
    AssertGL<>(m_gl);
}

template <class T, typename F>
    requires std::is_invocable_v<F, T*>
auto Buffer::Map(Access access, F&& func) noexcept(std::is_nothrow_invocable_v<F, T*>) -> decltype(auto) {
    auto ptr = m_handler.GetRaw();
    auto* data = reinterpret_cast<T*>(m_gl->MapNamedBuffer(ptr, static_cast<GLenum>(access)));
    AssertGL<>(m_gl);

    struct Guard {
        ~Guard() {
            m_gl->UnmapNamedBuffer(raw);
            AssertGL<>(m_gl);
        }

        GladGLContext* m_gl;
        GLuint raw;
    } guard{m_gl, ptr};

    return std::forward<F>(func)(data);
}
#pragma endregion

#pragma region Buffer Allocator implementation

BufferAllocator::BufferAllocator(Context& context) : m_gl(context.Get()) {}

auto BufferAllocator::New() noexcept(!lowl::Debug) -> Buffer::Id {
    GLuint result;
    m_gl->CreateBuffers(1, &result);
    AssertGL<>(m_gl);
    return Buffer::Id(result);
}

auto BufferAllocator::Delete(Buffer::Id id) noexcept(!lowl::Debug) -> void {
    if (id == Buffer::Id::Null) {
        return;
    }
    auto it = static_cast<GLuint>(id);
    Assert<>(m_gl->IsBuffer(it));

    m_gl->DeleteBuffers(1, &it);
    AssertGL<>(m_gl);
}
#pragma endregion

}    // namespace lowl::gl
#pragma endregion
