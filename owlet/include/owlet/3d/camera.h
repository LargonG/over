#pragma once

#include <owlet/3d/transform.h>

#include <glm/glm.hpp>

namespace owlet::d3 {

struct Camera {
  public:
    Camera(float aspect_ratio, glm::vec3 position, glm::vec3 target, float fov, float near, float far,
           SpaceSettings& st);

    glm::mat4 ViewMatrix() const noexcept;
    glm::mat4 ProjectionMatrix() const noexcept;
    glm::mat4 ViewProjectionMatrix() const noexcept;

    void SetPosition(glm::vec3 position);
    void SetTarget(glm::vec3 target);
    void SetSpace(SpaceSettings& space);
    void SetClip(glm::vec2 clip);
    void SetAspectRatio(float aspect_ratio);
    void SetFov(float fov);

    glm::vec3 Position() const noexcept { return m_position; }
    glm::vec3 Target() const noexcept { return m_target; }
    glm::vec2 Clip() const noexcept { return m_clip; }
    SpaceSettings& Space() const noexcept { return *m_st; }
    float AspectRatio() const noexcept { return m_aspect_ratio; }
    float Fov() const noexcept { return m_fov; }

  private:
    void UpdateViewMatrix() const noexcept;
    void UpdateProjectionMatrix() const noexcept;
    void UpdateViewProjectionMatrix() const noexcept;

    SpaceSettings* m_st;

    float m_aspect_ratio;
    float m_fov;

    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec2 m_clip;    // near, far

    mutable glm::mat4 m_view;    // cached
    mutable bool m_view_updated;

    mutable glm::mat4 m_projection;    // cached
    mutable bool m_projection_updated;

    mutable glm::mat4 m_view_projection;
};
}    // namespace owlet::d3
