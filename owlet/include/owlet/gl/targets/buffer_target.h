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
        auto* gl = m_buffer->GL();
        auto id = static_cast<GLuint>(m_buffer->Id());

        gl->BindBuffer(T::s_target, id);
        debug::GLCheckError(gl);

        return static_cast<T&>(*this);
    }

    T& Unbind() noexcept {
        auto* gl = m_buffer->GL();

        gl->BindBuffer(T::s_target, 0);    // warning: should be set to previous id
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

    Buffer& Owner() const noexcept { return *m_buffer; }

  protected:
    Buffer* m_buffer;
};

extern template BufferTarget<VertexBuffer>;
extern template BufferTarget<IndexBuffer>;
extern template BufferTarget<UniformBuffer>;

}    // namespace owlet::gl
