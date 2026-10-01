#include <owlet/3d/shape.h>

#include <span>
#include <vector>

#include <owlet/types.h>

namespace owlet::d3 {
std::vector<int32> ConvertElements(std::span<glm::ivec3> elements) {
    std::vector<int32> result;
    result.reserve(elements.size() * 3);

    for (const auto& element : elements) {
        result.push_back(element.x);
        result.push_back(element.y);
        result.push_back(element.z);
    }
    result.shrink_to_fit();

    return result;
}

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
