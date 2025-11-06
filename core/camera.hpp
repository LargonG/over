#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace over {
class Camera {
 public:
  Camera(glm::vec3 initialPosition, float initialYaw, float initialPitch,
         float initialFOV, glm::vec3 up);

  void UpdatePositionCallback(GLFWwindow* window, float deltaTime);
  void UpdateYawPitchCallback(float xpos, float ypos);
  void UpdateFOVCallback(float yoffset);

  glm::vec3 GetPosition() const noexcept { return position_; }
  glm::vec3 GetDirection() const noexcept;
  glm::vec3 GetUp() const noexcept { return up_; }

  glm::vec3& GetPosition() noexcept { return position_; }
  glm::vec3& GetUp() noexcept { return up_; }

  float Yaw() const noexcept { return yaw_; }
  float Pitch() const noexcept { return pitch_; }
  float FOV() const noexcept { return fov_; }

 private:
  float yaw_, pitch_;
  float fov_;

  glm::vec3 position_;
  glm::vec3 up_;
};
}  // namespace over