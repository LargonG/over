#pragma once

#include <functional>
#include <vector>

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/Binded.hpp>
#include <over/core/opengl/buffers/Buffer.hpp>

namespace over {

class Element {
 public:
  uint32 a, b, c;

  Element(uint32 a, uint32 b, uint32 c);

  Element(const Element&) = default;
};

class IBO : public Binded<IBO> {
 public:
  IBO();
  explicit IBO(std::vector<Element> elements, GLenum usage = GL_STATIC_DRAW);

  IBO(const IBO&) = default;
  IBO(IBO&&) noexcept = default;

  IBO& operator=(const IBO&) = default;
  IBO& operator=(IBO&&) noexcept = default;

  ~IBO() noexcept;

  void ToGPU(bool unbind = false);
  void FreeGPU() const noexcept;

  void Bind(bool copy = false) const;
  void Unbind() const;

  usize Size() { return _elements.size() * 3; }

  std::vector<Element>& GetElements() noexcept { return _elements; }
  const std::vector<Element>& GetElements() const noexcept { return _elements; }

 private:
  std::vector<Element> _elements;

  mutable Buffer<GL_ELEMENT_ARRAY_BUFFER> _buffer;
  mutable GLenum _usage;
};

}  // namespace over
