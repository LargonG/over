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
  BufferView() noexcept : _ptr(nullptr) {}
  BufferView(nullptr_t) : BufferView() {}
  explicit BufferView(Address ptr) noexcept : _ptr(ptr) {}

  BufferView(const BufferView&) = default;
  BufferView& operator=(const BufferView&) = default;

  BufferView(BufferView&&) noexcept = default;
  BufferView& operator=(BufferView&&) noexcept = default;

  ~BufferView() = default;

  void Bind() const { glthrow(glBindBuffer(_target, *_ptr)); }

  void Unbind() const { glthrow(glBindBuffer(_target, 0)); }

#pragma region Uniform
  // For Uniform & Transform feedback buffers ONLY

  void BindBase(usize index) {
    glthrow(glBindBufferBase(_target, static_cast<GLuint>(index), *_ptr));
  }

  void BindRange(usize index, usize offset, usize size) {
    glBindBufferRange(_target, static_cast<GLuint>(index), *_ptr,
                      static_cast<GLintptr>(offset),
                      static_cast<GLsizeiptr>(size));
  }

#pragma endregion

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
