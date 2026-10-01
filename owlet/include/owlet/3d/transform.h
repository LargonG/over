#pragma once

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

namespace owlet::d3 {

struct SpaceSettings;

struct Transform {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    Transform() noexcept;

    glm::vec3 Forward(SpaceSettings&) const noexcept;
    glm::vec3 Backward(SpaceSettings&) const noexcept;
    glm::vec3 Right(SpaceSettings&) const noexcept;
    glm::vec3 Left(SpaceSettings&) const noexcept;
};
}    // namespace owlet::d3
