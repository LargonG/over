#pragma once

#include <vector>

#include <owlet/3d/shape.h>

namespace owlet::d3 {
Shape Quad() {
    std::vector<glm::vec3> vbuf = {
        {-1, -1, 0},
        {1, -1, 0},
        {1, 1, 0},
        {-1, 1, 0},
    };
    std::vector<glm::ivec3> ibuf = {
        {0, 1, 2},
        {2, 3, 0},
    };

    return Shape{
        .vertices = std::move(vbuf),
        .elements = ConvertElements(ibuf),
        .strip = false,
    };
}

}    // namespace owlet::d3
