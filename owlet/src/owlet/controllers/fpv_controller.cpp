#include <owlet/controllers/fpv_controller.h>

#include <owlet/3d/core.h>

#include <glm/ext/quaternion_common.hpp>

namespace owlet::controllers {

FpvController::FpvController(float forward_speed, float side_speed, float backward_speed, float sensitivity,
                             d3::Transform* transform, d3::Camera* camera)
    : m_forward_speed(forward_speed),
      m_side_speed(side_speed),
      m_backward_speed(backward_speed),
      m_sensitivity(sensitivity),
      m_transform(transform),
      m_camera(camera) {}

void FpvController::Move(glm::vec2 position_direction, [[maybe_unused]] glm::vec2 rotation_direction) {
    auto pos_dir = glm::normalize(position_direction);
    auto rot_dir = glm::clamp(rotation_direction, glm::vec2(0, 0), glm::vec2(1, 1));

    auto first = m_transform->Forward(m_camera->Space());
    if (position_direction.x > 0) {
        first *= m_forward_speed;
    } else {
        first *= m_backward_speed;
    }

    auto second = m_transform->Right(m_camera->Space()) * m_side_speed;

    first *= position_direction.x;
    second *= position_direction.y;

    m_transform->position += first + second;
    m_camera->SetPosition(m_transform->position);

    float d_pitch = rot_dir.y / 2.0f;
    float d_yaw = rot_dir.x / 2.0f * m_camera->AspectRatio();

    glm::quat dw = glm::quat(glm::vec3(d_pitch, d_yaw, 0.0f) * m_camera->Fov() * m_sensitivity);

    m_transform->rotation = dw * m_transform->rotation;

    m_camera->SetTarget(m_transform->position + m_transform->Forward(m_camera->Space()));
}

}    // namespace owlet::controllers
