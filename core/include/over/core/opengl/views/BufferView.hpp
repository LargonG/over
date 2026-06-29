#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/Address.hpp>
#include <over/core/opengl/Binded.hpp>
#include <over/core/opengl/targets/BufferTarget.hpp>
#include <over/core/opengl/wrappers/BufferWrapper.hpp>
#include <over/core/opengl/wrappers/Exception.hpp>

namespace over::gl {

template <BufferTarget Target>
class BufferView : public Binded<BufferView<Target>> {
  constexpr static GLenum _target = static_cast<GLenum>(Target);

 public:
  BufferView() : _ptr(nullptr) {}
  BufferView(nullptr_t) : BufferView() {}

  BufferView(const BufferView&) = default;
  BufferView& operator=(const BufferView&) = default;

  BufferView(BufferView&&) noexcept = default;
  BufferView& operator=(BufferView&&) noexcept = default;

  ~BufferView() = default;

  template <class Allocator>
  explicit BufferView(const BufferWrapper<Allocator>* me) noexcept
      : _ptr(me->Get()) {}

  void Bind() const { glthrow(glBindBuffer(_target, *_ptr)); }

  void Unbind() const { glthrow(glBindBuffer(_target, 0)); }

  void Reserve(usize size, const void* data, GLenum usage) {
    if (0 == *_ptr) {
      return;
    }

    glthrow(glBufferData(_target, static_cast<GLsizeiptr>(size), data, usage));
  }

  void Write(usize offset, usize size, const void* data) {
    if (0 == *_ptr) {
      return;
    }

    glthrow(glBufferSubData(_target, static_cast<GLintptr>(offset),
                            static_cast<GLsizeiptr>(size), data));
  }

  void Clear(GLenum usage = GL_STATIC_DRAW) {
    if (0 == *_ptr) {
      return;
    }

    glthrow(glBufferData(_target, 0, nullptr, usage));
  }

  template <BufferTarget Other, typename = std::enable_if_t<Target != Other>>
  void Copy(BufferView<Other>& other) const {
    auto _size = GetParameter(GL_BUFFER_SIZE);
    glthrow(
        glCopyBufferSubData(_target, static_cast<GLenum>(Other), 0, 0, _size));
  }

  [[nodiscard]] GLint GetParameter(GLenum name) const {
    GLint res = 0;
    glthrow(glGetBufferParameteriv(_target, name, &res));
    return res;
  }

 private:
  Address _ptr;
};
}  // namespace over::gl
