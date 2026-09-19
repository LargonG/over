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
}    // namespace owlet::d3
