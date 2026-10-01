#include <owlet/3d/camera.h>

#include <owlet/3d/core.h>

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

namespace owlet::d3 {
Camera::Camera(float aspect_ratio, glm::vec3 position, glm::vec3 target, float fov, float near, float far,
               SpaceSettings& st)
    : m_st(&st),
      m_aspect_ratio(aspect_ratio),
      m_fov(fov),
      m_position(position),
      m_target(target),
      m_clip(near, far),
      m_view(),
      m_view_updated(true),
      m_projection(),
      m_projection_updated(true),
      m_view_projection() {}

glm::mat4 Camera::ViewMatrix() const noexcept {
    if (m_view_updated) {
        UpdateViewMatrix();
        UpdateViewProjectionMatrix();
        m_view_updated = false;
    }
    return m_view;
}

glm::mat4 Camera::ProjectionMatrix() const noexcept {
    if (m_projection_updated) {
        UpdateProjectionMatrix();
        UpdateViewProjectionMatrix();
        m_projection_updated = false;
    }
    return m_projection;
}

glm::mat4 Camera::ViewProjectionMatrix() const noexcept {
    ViewMatrix();
    ProjectionMatrix();
    return m_view_projection;
}

void Camera::SetPosition(glm::vec3 position) {
    m_view_updated = true;
    m_position = position;
}

void Camera::SetTarget(glm::vec3 target) {
    m_view_updated = true;
    m_target = target;
}

void Camera::SetSpace(SpaceSettings& st) {
    m_view_updated = true;
    m_st = &st;
}

void Camera::SetClip(glm::vec2 clip) {
    m_projection_updated = true;
    m_clip = clip;
}

void Camera::SetAspectRatio(float aspect_ratio) {
    m_projection_updated = true;
    m_aspect_ratio = aspect_ratio;
}

void Camera::SetFov(float fov) {
    m_projection_updated = true;
    m_fov = fov;
}

void Camera::UpdateViewMatrix() const noexcept {
    // position, target, up
    m_view = glm::lookAt(m_position, m_target, m_st->global_up);
}

void Camera::UpdateProjectionMatrix() const noexcept {
    m_projection = glm::perspective(m_fov, m_aspect_ratio, m_clip.x, m_clip.y);
}

void Camera::UpdateViewProjectionMatrix() const noexcept {
    m_view_projection = m_projection * m_view;
}

}    // namespace owlet::d3
