#pragma once

#include <owlet/gl/buffer.h>
#include <owlet/types.h>

namespace owlet::gl {
template <class T>
struct TypedBuffer {
  private:
    static constexpr usize value_size = sizeof(T);

  public:
    TypedBuffer(Buffer& buffer) : m_buffer(&buffer) {}

    TypedBuffer(const TypedBuffer&) = default;

    void Alloc(usize count, T* data, Buffer::Usage usage) {
        m_buffer->Alloc(count * value_size, reinterpret_cast<void*>(data), usage);
    }

    void Write(usize start_iterator, usize count, T* data) {
        m_buffer->Write(start_iterator * value_size, count * value_size, reinterpret_cast<void*>(data));
    }

    template <typename F>
        requires std::is_invocable_v<F, T*, usize>
    void Map(Buffer::Access access, F&& func) {
        m_buffer->Map<T>(access, std::forward<F>(func));
    }

  private:
    TypedBuffer() noexcept : m_buffer(nullptr) {}

    Buffer* m_buffer;
};
}    // namespace owlet::gl
