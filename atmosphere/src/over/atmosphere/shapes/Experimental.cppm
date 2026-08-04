module;

#include <vector>

#include <over/core/Types.hpp>

#include <fmt/core.h>

#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>

export module shapes:experimental;

namespace over::experimental {

using PolarMatrix = std::vector<std::vector<glm::vec2>>;

constexpr float32 r = 1.f;
constexpr float32 circle = 2.f * glm::pi<float32>();

static auto StartOffset(usize n, bool shift) -> std::tuple<glm::vec2, float32> {
  constexpr float32 r = 1.f;
  glm::vec2 result = glm::vec2(0.f, 0.f);

  auto medianAngle = circle / n;  // w
  auto x = r * glm::sqrt(1.f - glm::cos(medianAngle)) /
           glm::sqrt(1.5f - glm::cos(medianAngle));
  auto angle = glm::acos(1.f - 4.f * (1.f - glm::cos(medianAngle)) /
                                   (3.f - 2.f * glm::cos(medianAngle)));
  auto a = 2.f * x;

  auto cosAngle2 = glm::cos(angle / 2.f);
  auto cosMedian2 = glm::cos(medianAngle / 2.f);

  auto cosines = cosAngle2 * cosMedian2;

  auto first = 1.f / (2.f * cosines);
  auto second = cosines / 2.f;
  auto third = 3.f * x * x / (8.f * cosines);

  float32 cosine = glm::cos(angle / 2.f);

  auto horizontalAngle = glm::acos(2.f * cosine * (cosine - 1.f) + 1 / cosine);
  auto verticalAngle = glm::acos(first + second - third);

  result += glm::vec2(shift * horizontalAngle, verticalAngle);

  return {
      result,
      angle,
  };
}

static auto SpherePolarPoints(glm::vec2 start, float32 dAngle) -> PolarMatrix {
  constexpr float32 r = 1.f;
  PolarMatrix pts;

  float32 dl = glm::abs(r * glm::cos(start.y)) * dAngle;  // length

  while (start.y <= glm::pi<float32>() / 2.f) {
    float32 R = glm::abs(r * glm::cos(start.y));
    float32 L = circle * R;

    usize m = static_cast<usize>(L / dl);

    if (m == 0) {
      break;
    }
    pts.push_back({});

    float32 dx = circle / m;

    for (usize j = 0; j < m; j++) {
      glm::vec2 pt = start + glm::vec2(dx * j, 0.f);

      pts.back().push_back(pt);
    }

    float32 x = dx / 2.f;
    float32 cosX = glm::cos(x);

    // todo: maybe should be dx / 2.f, but results too high vertical angle
    float32 y = dAngle / 2.f;
    float32 cosY = glm::cos(y);

    float32 deltaX = glm::acos(2.f * cosX * (cosX - 1.f) + 1 / cosX);
    float32 deltaY = glm::acos(glm::cos(2.f * y) / cosY);

    start += glm::vec2(deltaX, deltaY);
  }

  return pts;
}

template <template <typename...> class R = std::vector, typename Top,
          typename Sub = typename Top::value_type>
static R<typename Sub::value_type> flatten(Top const& all) {
  using std::begin;
  using std::end;

  R<typename Sub::value_type> accum;

  for (auto& sub : all)
    accum.insert(end(accum), begin(sub), end(sub));

  return accum;
}

export auto VerticesData(std::vector<glm::vec2>& up,
                         std::vector<glm::vec2>& down)
    -> std::tuple<std::vector<glm::vec3>, std::vector<glm::vec3>> {

  usize size = up.size() + down.size();

  std::vector<glm::vec3> colors(size, glm::vec3(0.f));
  std::vector<glm::vec3> xyz(size, glm::vec3(0.f));

  for (usize i = 0; i < size; i++) {
    auto pt = glm::vec2();
    if (i < up.size()) {
      pt = up[i];
    } else {
      pt = -down[i - down.size()];
    }

    xyz[i] = glm::vec3(r * glm::cos(pt.x) * glm::cos(pt.y),  // x
                       r * glm::sin(pt.y),                   // y
                       r * glm::sin(pt.x) * glm::cos(pt.y)   // z
    );

    colors[i] = (xyz[i] + 1.f) / 2.f;
  }

  return {std::move(xyz), std::move(colors)};
}

export auto Balance(const std::vector<glm::ivec3>& elements,
                    std::vector<glm::vec3>& pts, usize iterations, float32 k) {
  std::vector<glm::vec3> forces(pts.size());

  for (usize it = 0; it < iterations; it++) {
    std::fill(forces.begin(), forces.end(), glm::vec3(0.f));
    for (usize i = 0; i < elements.size(); i++) {
      for (usize j = 0; j < 3; j++) {
        auto prev = elements[i][(j + 3 - 1) % 3];
        auto cur = elements[i][j];
        auto next = elements[i][(j + 1) % 3];

        auto to_next = pts[next] - pts[cur];
        auto to_prev = pts[prev] - pts[cur];

        forces[cur] += to_next * k;
        forces[cur] += to_prev * k;
      }
    }

    for (usize i = 0; i < pts.size(); i++) {
      pts[i] += forces[i] * ((iterations - it) * 1.f / iterations);
      pts[i] = glm::normalize(pts[i]);
    }
  }
}

static auto AddPolusElements(std::vector<glm::ivec3>& result,
                             const std::vector<glm::vec2>& pts, usize offset) {
  usize l = 0;
  usize r = pts.size() - 1;

  bool step = false;

  while (l + 2 <= r) {
    usize first = l;
    usize next = 0;
    usize second = r;

    if (step) {
      next = l + 1;
      l++;
    } else {
      next = r - 1;
      r--;
    }

    result.push_back(glm::ivec3(first, second, next) + glm::ivec3(offset));

    step = !step;
  }
}

static auto GlobalIndex(usize i, usize layer_size, usize offset) {
  return offset + i % layer_size;
}

static auto Current(usize i, usize layer_size, usize offset) {
  return GlobalIndex(i, layer_size, offset);
}

static auto Next(usize i, usize layer_size, usize offset) {
  return GlobalIndex(i + 1, layer_size, offset);
}

static auto Up(usize i, usize layer_size, usize up_layer_size, usize offset) {
  auto k = up_layer_size * 1.f / layer_size;
  auto up_i = static_cast<usize>(std::round(i * k));

  return GlobalIndex(up_i, up_layer_size, offset + layer_size);
}

static auto Down(usize i, usize down_layer_size, usize offset, usize cur,
                 usize next, const std::vector<glm::ivec3>& elements) {

  usize down = 0;
  auto down_start = elements.size() - i - down_layer_size;
  for (usize k = 0; k < down_layer_size; k++) {
    if (elements[down_start + k].z == cur &&
        elements[down_start + ((k + 1) % down_layer_size)].z == next) {
      down = Next(k, down_layer_size, offset - down_layer_size);
      break;
    }
  }

  return down;
}

export auto Elements(std::vector<glm::ivec3>& result, const PolarMatrix& pts,
                     usize offset) {

  usize start_size = result.size();
  usize size = 0;

  for (usize i = 0; i < pts.size(); i++) {
    auto cur_layer_size = pts[i].size();

    if (i > 0) {
      auto down_layer_size = pts[i - 1].size();
      for (usize j = 0; j < cur_layer_size; j++) {
        auto cur = Current(j, cur_layer_size, size);
        auto next = Next(j, cur_layer_size, size);

        auto down = Down(j, down_layer_size, size, cur, next, result);

        result.push_back(glm::ivec3(cur, next, down));
      }
    }

    if (i < pts.size() - 1) {
      auto up_layer_size = pts[i + 1].size();
      for (usize j = 0; j < cur_layer_size; j++) {
        auto cur = Current(j, cur_layer_size, size);
        auto next = Next(j, cur_layer_size, size);

        auto up = Up(j, cur_layer_size, up_layer_size, size);

        result.push_back(glm::ivec3(next, cur, up));
      }
    }

    size += pts[i].size();
  }

  AddPolusElements(result, pts.back(), size - pts.back().size());

  for (usize i = start_size; i < result.size(); i++) {
    result[i] += offset;
  }
}

auto MergeHemispheres(const PolarMatrix& up_pts, const PolarMatrix& down_pts,
                      std::vector<glm::ivec3>& result, usize separator) {
  for (usize j = 0; j < up_pts.front().size(); j++) {
    auto cur = j;
    auto next = (j + 1) % up_pts.front().size();

    auto down = (down_pts.front().size() - j - 1);

    result.push_back(glm::ivec3(cur, next, down + separator));
  }

  for (usize j = 0; j < down_pts.front().size(); j++) {
    auto cur = j;
    auto next = (j + 1) % down_pts.front().size();

    auto up = (up_pts.front().size() - j - 1);

    result.push_back(glm::ivec3(cur + separator, next + separator, up));
  }
}

export auto Sphere(usize n)
    -> std::tuple<std::vector<glm::vec3>, std::vector<glm::vec3>,
                  std::vector<glm::ivec3>> {

  auto [upStart, upAngle] = StartOffset(n, false);
  auto [downStart, downAngle] = StartOffset(n, true);

  auto ptsUp = SpherePolarPoints(upStart, upAngle);
  auto ptsDown = SpherePolarPoints(downStart, downAngle);

  auto up = flatten(ptsUp);
  auto down = flatten(ptsDown);
  auto size = up.size() + down.size();

  auto [vertices, colors] = VerticesData(up, down);

  std::vector<glm::ivec3> elements;
  Elements(elements, ptsUp, 0);
  Elements(elements, ptsDown, up.size());
  MergeHemispheres(ptsUp, ptsDown, elements, up.size());

  return {std::move(vertices), std::move(colors), std::move(elements)};
}

}  // namespace over::experimental
