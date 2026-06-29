#include <over/core/opengl/VAO.hpp>

#include <cassert>
#include <utility>

#include <over/core/Includes.hpp>

namespace over {

void VAO::AttachAttribute(uint32 location, uint32 count, GLenum type,
                          usize size, usize offset) {

  auto view = _layout.As<gl::LayoutTarget::VERTEX_ARRAY>();
  view.SetAttribute(location, count, type, size, offset);
  view.EnableAttribute(location);
}

void VAO::Bind() const {
  _layout.As<gl::LayoutTarget::VERTEX_ARRAY>().Bind();
}

void VAO::Unbind() const noexcept {
  _layout.As<gl::LayoutTarget::VERTEX_ARRAY>().Unbind();
}
}  // namespace over
