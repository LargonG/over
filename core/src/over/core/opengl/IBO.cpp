#include <over/core/opengl/IBO.hpp>

#include <cassert>
#include <utility>

namespace over {
Element::Element(uint32 a, uint32 b, uint32 c) : a(a), b(b), c(c) {}

IBO::IBO() : _buffer(), _elements(), _usage(GL_STATIC_DRAW) {}

IBO::IBO(std::vector<Element> elements, GLenum usage)
    : _buffer(), _elements(std::move(elements)), _usage(usage) {}

IBO::~IBO() noexcept {
  FreeGPU();
}

void IBO::ToGPU(bool unbind) {
  _buffer.Alloc();

  Bind(true);

  if (unbind) {
    Unbind();
  }
}

void IBO::FreeGPU() const noexcept {
  _buffer.Free();
}

void IBO::Bind(bool copy) const {
  _buffer.Bind();

  if (copy) {
    _buffer.Reserve(sizeof(Element) * _elements.size(),
                    reinterpret_cast<const ubyte*>(_elements.data()), _usage);
  }
}

void IBO::Unbind() const {
  _buffer.Unbind();
}
}  // namespace over
