#pragma once

#include <owlet/3d/camera.h>

#include <glm/glm.hpp>

namespace owlet::controllers {
struct FpvController {
  public:
    FpvController(float forward_speed, float side_speed, float backward_speed, float sensitivity,
                  d3::Transform* transform, d3::Camera* camera);

    void Move(glm::vec2 position_direction, glm::vec2 rotation_direction);

  private:
    d3::Transform* m_transform;
    d3::Camera* m_camera;

    float m_forward_speed;
    float m_side_speed;
    float m_backward_speed;
    float m_sensitivity;
};
}    // namespace owlet::controllers
