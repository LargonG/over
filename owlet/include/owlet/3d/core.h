#pragma once

#include <owlet/3d/camera.h>
#include <owlet/3d/shape.h>
#include <owlet/3d/transform.h>

#include <glm/glm.hpp>

namespace owlet::d3 {
struct SpaceSettings {
    glm::vec3 global_up;
    glm::vec3 global_right;

    glm::vec3 GlobalForward() const { return glm::cross(global_up, global_right); }
};
}    // namespace owlet::d3
