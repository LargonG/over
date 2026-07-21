#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/Address.hpp>
#include <over/core/opengl/targets/FrameBufferTarget.hpp>
#include <over/core/opengl/targets/RenderBufferTarget.hpp>
#include <over/core/opengl/wrappers/Exception.hpp>
#include <over/core/opengl/wrappers/RenderBufferWrapper.hpp>

namespace over::gl {
template <RenderBufferTarget Target>
class RenderBufferView : public Binded<RenderBufferView<Target>> {
 private:
  constexpr static GLenum _target = static_cast<GLenum>(Target);

 public:
  RenderBufferView() noexcept : _ptr(nullptr) {}
  RenderBufferView(nullptr_t) noexcept : RenderBufferView() {}
  explicit RenderBufferView(Address ptr) noexcept : _ptr(ptr) {}

  RenderBufferView(const RenderBufferView&) = default;
  RenderBufferView& operator=(const RenderBufferView&) = default;

  RenderBufferView(RenderBufferView&&) noexcept = default;
  RenderBufferView& operator=(RenderBufferView&&) noexcept = default;

  ~RenderBufferView() = default;

  void Bind() const { glthrow(glBindRenderbuffer(_target, *_ptr)); }

  void Unbind() const { glthrow(glBindRenderbuffer(_target, 0)); }

  void Reserve(GLenum format, usize width, usize height) {
    glthrow(glRenderbufferStorage(_target, format, static_cast<GLsizei>(width),
                                  static_cast<GLsizei>(height)));
  }

  void ReserveMultisample(usize samples, GLenum format, usize width,
                          usize height) {
    glthrow(glRenderbufferStorageMultisample(_target, samples, format, width,
                                             height));
  }

 private:
  Address _ptr;

  template <FrameBufferTarget>
  friend class FrameBufferView;
};
}  // namespace over::gl
