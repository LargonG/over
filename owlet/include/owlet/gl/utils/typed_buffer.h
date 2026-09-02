#pragma once

#include <owlet/gl/buffer.h>
#include <owlet/types.h>

namespace owlet::gl {
template <class T>
struct TypedBuffer {
  private:
    static constexpr usize value_size = sizeof(T);

  public:
    explicit TypedBuffer(Buffer& buffer) : m_buffer(&buffer) {}

    TypedBuffer(const TypedBuffer&) = default;

    TypedBuffer& Alloc(usize count, T* data, Buffer::Usage usage) {
        m_buffer->Alloc(count * value_size, reinterpret_cast<void*>(data), usage);
        return *this;
    }

    TypedBuffer& Write(usize start_iterator, usize count, T* data) {
        m_buffer->Write(start_iterator * value_size, count * value_size, reinterpret_cast<void*>(data));
        return *this;
    }

    template <typename F>
        requires std::is_invocable_v<F, T*, usize>
    TypedBuffer& Map(Buffer::Access access, F&& func) {
        m_buffer->Map<T>(access, std::forward<F>(func));
        return *this;
    }

    template <class T, typename F>
    T As(F&& func) {
        return m_buffer->As<T, F>(std::forward<F>(func));
    }

    template <class T>
    T As() {
        return m_buffer->As<T>();
    }

  private:
    TypedBuffer() noexcept : m_buffer(nullptr) {}

    Buffer* m_buffer;
};
}    // namespace owlet::gl
