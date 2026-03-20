#include <over/core/opengl/IBO.hpp>

#include <cassert>
#include <utility>

namespace over {
Element::Element(uint32 a, uint32 b, uint32 c) : a(a), b(b), c(c) {}

IBO::IBO() : _id(0), _elements(), _usage(GL_STATIC_DRAW) {}

IBO::IBO(std::vector<Element> elements, GLenum usage)
    : _id(0), _elements(std::move(elements)), _usage(usage) {}

IBO::IBO(const IBO& other) : IBO() {
  *this = other;
}

IBO::IBO(IBO&& other) noexcept : IBO() {
  *this = std::move(other);
}

IBO& IBO::operator=(const IBO& other) {
  if (this == &other) {
    return *this;
  }

  // TODO: add copy on write
  _elements = other._elements;
  _usage = other._usage;

  return *this;
}

IBO& IBO::operator=(IBO&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  _id = std::exchange(other._id, 0);
  _elements = std::move(other._elements);
  _usage = std::exchange(other._usage, GL_STATIC_DRAW);

  return *this;
}

IBO::~IBO() noexcept {
  FreeGPU();
}

void IBO::ToGPU(bool unbind) {
  //assert(_elements.size() > 0);

  // TODO: add method to borrow id from other ids allocator
  if (_id == 0) {
    glGenBuffers(1, &_id);
  }

  Bind(true);

  if (unbind) {
    Unbind();
  }
}

void IBO::FreeGPU() const noexcept {
  if (_id == 0) {
    return;
  }

  glDeleteBuffers(1, &_id);
  _id = 0;
}

void IBO::Bind(bool copy) const {
  assert(_id != 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
  if (copy) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Element) * _elements.size(),
                 _elements.data(), _usage);
  }
}

void IBO::Unbind() const {
  assert(_id != 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}
}  // namespace over
