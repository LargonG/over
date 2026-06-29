#pragma once

#include <functional>
#include <vector>

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/Binded.hpp>
#include <over/core/opengl/views/BufferView.hpp>
#include <over/core/opengl/wrappers/BufferWrapper.hpp>

namespace over {

class Element {
 public:
  uint32 a, b, c;

  Element(uint32 a, uint32 b, uint32 c);

  Element(const Element&) = default;
};

class IBO : public Binded<IBO> {
 public:
  IBO() = default;
  explicit IBO(std::vector<Element> elements, GLenum usage = GL_STATIC_DRAW);

  IBO(const IBO&) = delete;
  IBO& operator=(const IBO&) = delete;

  IBO(IBO&&) noexcept = default;
  IBO& operator=(IBO&&) noexcept = default;

  ~IBO() = default;

  void ToGPU(bool unbind = false);

  void Bind(bool copy = false) const;
  void Unbind() const;

  usize Size() { return _elements.size() * 3; }

  std::vector<Element>& GetElements() noexcept { return _elements; }
  const std::vector<Element>& GetElements() const noexcept { return _elements; }

 private:
  std::vector<Element> _elements;

  gl::BufferWrapper<> _buffer;
  GLenum _usage;
};

}  // namespace over
