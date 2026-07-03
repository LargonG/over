#pragma once

#include <over/core/Types.hpp>
#include <over/core/opengl/Address.hpp>
#include <over/core/opengl/Binded.hpp>
#include <over/core/opengl/targets/LayoutTarget.hpp>
#include <over/core/opengl/wrappers/Exception.hpp>
#include <over/core/opengl/wrappers/LayoutWrapper.hpp>

namespace over::gl {
template <LayoutTarget Target>
class LayoutView : public Binded<LayoutView<Target>> {
 public:
  LayoutView() noexcept : _ptr(nullptr) {}
  LayoutView(nullptr_t) noexcept : LayoutView() {}
  explicit LayoutView(Address ptr) noexcept : _ptr(ptr) {}

  LayoutView(const LayoutView&) = default;
  LayoutView& operator=(const LayoutView&) = default;

  LayoutView(LayoutView&&) noexcept = default;
  LayoutView& operator=(LayoutView&&) noexcept = default;

  ~LayoutView() = default;

  void Bind() const { glthrow(glBindVertexArray(*_ptr)); }

  void Unbind() const { glthrow(glBindVertexArray(0)); }

  void SetAttribute(usize index, int32 size, GLenum type, usize shift,
                    usize offset) {
    glthrow(glVertexAttribPointer(
        static_cast<GLuint>(index), static_cast<GLint>(size), type, GL_FALSE,
        static_cast<GLsizei>(shift), reinterpret_cast<void*>(offset)));
  }

  void EnableAttribute(usize index) {
    assert(index < GL_MAX_VERTEX_ATTRIBS);
    glthrow(glEnableVertexAttribArray(index));
  }

  void DisableAttribute(usize index) {
    assert(index < GL_MAX_VERTEX_ATTRIBS);
    glthrow(glDisableVertexAttribArray(index));
  }

 private:
  Address _ptr;
};
}  // namespace over::gl
