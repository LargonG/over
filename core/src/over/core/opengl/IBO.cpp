#include <over/core/opengl/IBO.hpp>

#include <cassert>
#include <utility>

namespace over {
Element::Element(uint32 a, uint32 b, uint32 c) : a(a), b(b), c(c) {}

IBO::IBO(std::vector<Element> elements, GLenum usage)
    : _buffer(), _elements(std::move(elements)), _usage(usage) {}

void IBO::ToGPU(bool unbind) {
  Bind(true);

  if (unbind) {
    Unbind();
  }
}

void IBO::Bind(bool copy) const {
  auto view = _buffer.As<gl::BufferTarget::ELEMENT_ARRAY_BUFFER>();

  view.Bind();

  if (copy) {
    // legacy
    const_cast<gl::BufferView<gl::BufferTarget::ELEMENT_ARRAY_BUFFER>&>(view)
        .Reserve(sizeof(Element) * _elements.size(),
                 static_cast<const void*>(_elements.data()), _usage);
  }
}

void IBO::Unbind() const {
  _buffer.As<gl::BufferTarget::ELEMENT_ARRAY_BUFFER>().Unbind();
}
}  // namespace over
