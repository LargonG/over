module;

#include <numbers>
#include <vector>

#include <over/core/Types.hpp>
#include <over/core/opengl/views/BufferView.hpp>
#include <over/core/opengl/views/LayoutView.hpp>
#include <over/core/opengl/wrappers/BufferWrapper.hpp>
#include <over/core/opengl/wrappers/LayoutWrapper.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/polar_coordinates.hpp>

export module shapes;

import :experimental;

namespace over {
export class Sphere {
 public:
  Sphere() noexcept = default;

  Sphere(const Sphere&) = delete;
  Sphere& operator=(const Sphere&) = delete;

  Sphere(Sphere&&) noexcept = default;
  Sphere& operator=(Sphere&&) noexcept = default;

  Sphere(usize n, bool inverted) { Generate(n, inverted); }

  auto Layout() const -> gl::LayoutView<gl::LayoutTarget::VERTEX_ARRAY> {
    return _layout.As<gl::LayoutTarget::VERTEX_ARRAY>();
  }

  auto Vertices() const -> gl::BufferView<gl::BufferTarget::ARRAY_BUFFER> {
    return _geometry.As<gl::BufferTarget::ARRAY_BUFFER>();
  }

  auto Elements() const
      -> gl::BufferView<gl::BufferTarget::ELEMENT_ARRAY_BUFFER> {
    return _elements.As<gl::BufferTarget::ELEMENT_ARRAY_BUFFER>();
  }

  auto VerticesCount() const noexcept { return _geometryCount; }

  auto ElementsCount() const noexcept { return _elementsCount; }

 private:
  auto Generate(usize n, bool inverted) -> void {
    auto [vertices, colors, elements] = experimental::Sphere(n);

    experimental::Balance(elements, vertices, static_cast<usize>(5), 0.02f);

    if (inverted) {
      for (usize i = 0; i < elements.size(); i++) {
        std::swap(elements[i].x, elements[i].z);
      }
    }

    _geometry.As<gl::BufferTarget::ARRAY_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::ARRAY_BUFFER> self) {
          self.Reserve(sizeof(vertices.back()) * vertices.size(),
                       vertices.data(), GL_STATIC_DRAW);
        });

    _colors.As<gl::BufferTarget::ARRAY_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::ARRAY_BUFFER> self) {
          self.Reserve(sizeof(colors.back()) * colors.size(), colors.data(),
                       GL_STATIC_DRAW);
        });

    _elements.As<gl::BufferTarget::ELEMENT_ARRAY_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::ELEMENT_ARRAY_BUFFER> self) {
          self.Reserve(sizeof(elements.back()) * elements.size(),
                       elements.data(), GL_STATIC_DRAW);
        });

    _layout.As<gl::LayoutTarget::VERTEX_ARRAY>(
        [&](gl::LayoutView<gl::LayoutTarget::VERTEX_ARRAY> self) {
          _geometry.As<gl::BufferTarget::ARRAY_BUFFER>().Bind();
          _elements.As<gl::BufferTarget::ELEMENT_ARRAY_BUFFER>().Bind();

          // geometry
          self.EnableAttribute(0);
          self.SetAttribute(0, 3, GL_FLOAT, 0, 0);

          _colors.As<gl::BufferTarget::ARRAY_BUFFER>().Bind();
          self.EnableAttribute(1);
          self.SetAttribute(1, 3, GL_FLOAT, 0, 0);
        });

    _geometry.As<gl::BufferTarget::ARRAY_BUFFER>().Unbind();
    _elements.As<gl::BufferTarget::ELEMENT_ARRAY_BUFFER>().Unbind();

    _geometryCount = vertices.size();
    _elementsCount = elements.size();
    //fmt::println("geom count: {}", _geometryCount);
    //fmt::println("elems count: {}", _elementsCount);
  }

  gl::LayoutWrapper<> _layout;
  gl::BufferWrapper<> _geometry;
  gl::BufferWrapper<> _elements;
  gl::BufferWrapper<> _colors;

  usize _geometryCount;
  usize _elementsCount;
};
}  // namespace over
