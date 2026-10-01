#include <owlet/3d/transform.h>

#include <owlet/3d/core.h>

#include <glm/glm.hpp>

namespace owlet::d3 {
Transform::Transform() noexcept : position(0.0f), rotation(0.0f, 0.0f, 0.0f, 0.0f), scale(1.0f) {}

glm::vec3 Transform::Forward(SpaceSettings& st) const noexcept {
    auto forward = st.GlobalForward();
    auto m = glm::toMat4(rotation) * glm::vec4(forward, 1.0f);
    auto direction = glm::normalize(glm::vec3(m));
    return direction;
}

glm::vec3 Transform::Backward(SpaceSettings& st) const noexcept {
    return -Forward(st);
}

glm::vec3 Transform::Right(SpaceSettings& st) const noexcept {
    auto m = glm::toMat4(rotation) * glm::vec4(st.global_right, 1.0f);
    auto direction = glm::normalize(glm::vec3(m));
    return direction;
}

glm::vec3 Transform::Left(SpaceSettings& st) const noexcept {
    return -Right(st);
}
}    // namespace owlet::d3
