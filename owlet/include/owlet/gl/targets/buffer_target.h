#pragma once
#include <owlet/debug/gl.h>

/// Use to create BufferTarget implementation
#define WL_DEF_BUFFER_TARGET(target) constexpr static GLenum s_target = target

namespace owlet::gl {

struct VertexBuffer;
struct IndexBuffer;
struct UniformBuffer;

struct Buffer;

template <class T>
struct BufferTarget {
  public:
    BufferTarget() noexcept : m_buffer(nullptr) {}
    BufferTarget(Buffer& buffer) noexcept : m_buffer(&buffer) {}

    BufferTarget(const BufferTarget&) = default;

    T& Bind() noexcept {
        auto* gl = GL();
        auto id = Id();
        gl->BindBuffer(T::s_target, id);
        debug::GLCheckError(gl);
        return static_cast<T&>(*this);
    }

    T& Unbind() noexcept {
        auto* gl = GL();
        gl->BindBuffer(T::s_target, 0);
        debug::GLCheckError(gl);
        return static_cast<T&>(*this);
    }

    template <typename F>
    T& Do(F&& func) {
        Bind();
        std::forward<F>(func)();
        Unbind();

        return static_cast<T&>(*this);
    }

  protected:
    Buffer* m_buffer;

    auto Id() const noexcept { return m_buffer->m_handler.GetRaw(); }
    auto GL() const noexcept { return m_buffer->m_handler.Context(); }
};

extern template BufferTarget<VertexBuffer>;
extern template BufferTarget<IndexBuffer>;
extern template BufferTarget<UniformBuffer>;

}    // namespace owlet::gl
