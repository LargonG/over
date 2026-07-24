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

namespace {
auto GeneratePoints(over::usize n)
    -> std::tuple<std::vector<glm::vec3>, std::vector<glm::vec3>>;
auto GenerateElements(over::usize n) -> std::vector<glm::ivec3>;
}  // namespace

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
    auto [vertices, colors] = GeneratePoints(n);
    auto elements = GenerateElements(n);

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

module :private;
namespace {

using over::int64;
using over::usize;

auto GeneratePoints(usize n)
    -> std::tuple<std::vector<glm::vec3>, std::vector<glm::vec3>> {
  std::vector<glm::vec3> result;
  result.reserve(n * n);
  std::vector<glm::vec3> colors;

  auto maxN = static_cast<int64>(n / 4);
  auto minN = -maxN;

  auto etalonAngle = glm::radians(360.f) / n;

  auto offset = glm::vec2(0.f);

  for (int64 i = minN; i <= maxN; i++) {
    auto circleRadius = glm::abs(glm::cos(etalonAngle * i));
    auto circleLen = 2 * glm::pi<float>() * circleRadius;

    auto m = static_cast<usize>(circleLen / etalonAngle);

    if (m == 0) {
      continue;
    }

    auto currentAngle = glm::radians(360.f) / m;
    auto currentLen = 2 * glm::abs(glm::sin(currentAngle / 2));
    //fmt::println("circleRadius = {}, circleLen = {}, etalonAngle = {}, m = {}",
    //             circleRadius, circleLen, etalonAngle, m);

    for (usize j = 0; j < m; j++) {
      auto pt = offset + glm::vec2(etalonAngle * i, currentAngle * j);

      result.push_back(glm::vec3(glm::euclidean(pt)));
      auto v = result.back();
      colors.push_back((v + glm::vec3(1.f)) / 2.f);
      //colors.push_back(glm::vec3(
      //    (pt / glm::vec2(glm::radians(360.f)) + glm::vec2(1.f)) / 2.f,
      //    (v.z + 1) / 2));
    }

    offset += glm::vec2(0, currentLen / 2);
  }

  return {result, colors};
}

auto AddPolusTriangle(std::vector<glm::ivec3>& elements, usize size, usize n) {
  //fmt::println("{} {}", elements.size(), n);
  auto start = glm::ivec3(size - n);
  bool reversed = start.x > 0;
  for (usize j = 0; j < n / 2; j++) {
    auto l = j;
    auto r = (n - j) % n;

    auto l1 = (j + 1) % n;
    auto r1 = (r + n - 1) % n;

    auto first = glm::ivec3(r, r1, l1) + start;
    auto second = glm::ivec3(l1, l, r) + start;

    if (reversed) {
      std::swap(first.x, first.z);
      std::swap(second.x, second.z);
    }

    if (l1 != r1) {
      elements.push_back(first);
    }
    if (r != l) {
      elements.push_back(second);
    }
  }
}

auto GenerateElements(usize n) -> std::vector<glm::ivec3> {
  std::vector<glm::ivec3> elements;
  elements.reserve(2 * n * n);

  auto maxN = static_cast<int64>(n / 4);
  auto minN = -maxN;

  auto etalonAngle = glm::radians(360.f) / n;

  usize previousStart = 0;
  usize previousM = 0;
  usize size = 0;

  for (int64 i = minN; i <= maxN; i++) {
    auto circleRadius = glm::abs(glm::cos(etalonAngle * i));
    auto circleLen = 2 * glm::pi<float>() * circleRadius;

    auto m = static_cast<usize>(circleLen / etalonAngle);
    auto start = size;

    auto currentAngle = glm::radians(360.f) / m;
    auto previousAngle = glm::radians(360.f) / previousM;

    bool topPart = previousM > m;
    if (topPart) {
      std::swap(previousM, m);
      std::swap(previousStart, start);
      std::swap(currentAngle, previousAngle);
    }

    if (previousM > 0 && m > 0) {
      for (usize j = 0; j < m; j++) {
        auto it = start + j;
        auto right = start + ((j + 1) % m);

        auto downLeft =
            previousStart +
            static_cast<usize>((j * currentAngle + previousAngle / 2.f) /
                               previousAngle) %
                previousM;
        auto downRight =
            previousStart +
            static_cast<usize>(((j + 1) * currentAngle + previousAngle / 2.f) /
                               previousAngle) %
                previousM;

        auto mainTriangle = glm::ivec3(downLeft, right, it);
        auto fillTriangle = glm::ivec3(downLeft, downRight, right);

        if (topPart) {
          std::swap(mainTriangle.x, mainTriangle.z);
          std::swap(fillTriangle.x, fillTriangle.z);
        }

        elements.push_back(mainTriangle);
        if (downLeft != downRight) {
          elements.push_back(fillTriangle);
        }
      }
    }

    if (topPart) {
      std::swap(previousM, m);
      std::swap(previousStart, start);
      std::swap(currentAngle, previousAngle);
    }

    size += m;
    //fmt::println("m = {}, previous m = {}", m, previousM);
    if (previousM == 0 && m > 0 || previousM > 0 && m == 0 || i == maxN) {
      AddPolusTriangle(elements, size, m == 0 ? previousM : m);
    }

    previousM = m;
    previousStart = start;
  }

  return elements;
}
}  // namespace
