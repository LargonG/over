#pragma once

#include <span>
#include <vector>

#include <owlet/types.h>

#include <glm/glm.hpp>

namespace owlet::d3 {
struct Shape {
    std::vector<glm::vec3> vertices;
    std::vector<int32> elements;
    bool strip = false;

    usize VerticesInBytes() const noexcept { return sizeof(decltype(vertices[0])) * vertices.size(); }

    usize ElementsInBytes() const noexcept { return sizeof(decltype(elements[0])) * elements.size(); }
};

std::vector<int32> ConvertElements(std::span<glm::ivec3> elements);

Shape Quad();

}    // namespace owlet::d3
