#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>

#include <glm/glm.hpp>

namespace over {
class Camera {
 public:
  Camera(glm::vec3 initialPosition, float32 initialYaw, float32 initialPitch,
         float32 initialFOV, glm::vec3 up, float32 aspectRatio,
         float32 near = 0.1f, float32 far = 1000.f);

  void UpdatePositionCallback(GLFWwindow* window, float32 deltaTime);
  void UpdateYawPitchCallback(float32 xpos, float32 ypos, bool flush = false);
  void UpdateFOVCallback(float32 yoffset);

  glm::mat4 GetView() const noexcept;
  glm::mat4 GetProjection() const noexcept;

  glm::vec3 GetPosition() const noexcept { return _position; }
  glm::vec3 GetDirection() const noexcept;
  glm::vec3 GetUp() const noexcept { return _up; }

  glm::vec3& GetPosition() noexcept { return _position; }
  glm::vec3& GetUp() noexcept { return _up; }

  float Yaw() const noexcept { return _yaw; }
  float Pitch() const noexcept { return _pitch; }
  float FOV() const noexcept { return _fov; }

 private:
  float32 _yaw, _pitch;
  float32 _fov;

  float32 _aspectRatio;

  float32 _near, _far;

  glm::vec3 _position;
  glm::vec3 _up;
};
}  // namespace over