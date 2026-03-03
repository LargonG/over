#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>

#include <glm/glm.hpp>

namespace over {
class Camera {
 public:
  Camera(glm::vec3 position, glm::vec3 rotation, float32 speed, float32 fov,
         float32 aspectRatio, float32 near = 0.1f, float32 far = 1000.f,
         float32 sensitivity = 0.1f);
  static glm::vec3 GetUp() { return up; }
  static glm::vec3 SetUp(glm::vec3 newUp) { up = newUp; }

  void Camera::UpdatePositionCallback(GLFWwindow* window, float32 deltaTime);
  void Camera::UpdateYawPitchCallback(float32 xpos, float32 ypos,
                                      bool flush = false);
  void Camera::UpdateFOVCallback(float32 yoffset);

  glm::mat4 GetView() const noexcept;
  glm::mat4 GetProjection() const noexcept;

  glm::vec3 GetPosition() const noexcept { return _position; }
  glm::vec3 GetDirection() const noexcept;

  glm::vec3 GetRotation() const noexcept { return _rotation; }
  glm::vec3 GetRotationDegrees() const noexcept {
    return glm::degrees(_rotation);
  }
  void SetRotation(glm::vec3 rotation) { _rotation = rotation; }
  void SetRotationDegrees(glm::vec3 rotationDegrees) {
    _rotation = glm::radians(rotationDegrees);
  }

  glm::vec3& GetPosition() noexcept { return _position; }

  float32 GetFovDegrees() const noexcept { return glm::degrees(_fov); }
  float32 GetFov() const noexcept { return _fov; }

  float32 GetAspectRation() const noexcept { return _aspectRatio; }
  void SetAspectRatio(float32 aspectRatio) noexcept {
    _aspectRatio = aspectRatio;
  }

  float32 GetSpeed() const noexcept { return _speed; }
  void SetSpeed(float32 speed) noexcept { _speed = speed; }

 private:
  static glm::vec3 up;

  glm::vec3 _position;
  glm::vec3 _rotation;
  float32 _speed;
  float32 _sensitivity;

  float32 _aspectRatio;
  float32 _fov;

  float32 _near, _far;
};
}  // namespace over