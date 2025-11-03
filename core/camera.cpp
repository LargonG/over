#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace over {
Camera::Camera(glm::vec3 initialPosition, float initialYaw, float initialPitch,
               float initialFOV, glm::vec3 up)
    : position_(std::move(initialPosition)),
      yaw_(initialYaw),
      pitch_(initialPitch),
      fov_(initialFOV),
      up_(std::move(up)) {}

void Camera::UpdatePositionCallback(GLFWwindow* window, float deltaTime) {
  const float cameraSpeed = 5.0f;
  auto direction = GetDirection();
  glm::vec3 result(0.0f, 0.0f, 0.0f);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    result += deltaTime * cameraSpeed * direction;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    result -= deltaTime * cameraSpeed * direction;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    result -=
        deltaTime * cameraSpeed * glm::normalize(glm::cross(direction, up_));
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    result +=
        deltaTime * cameraSpeed * glm::normalize(glm::cross(direction, up_));
  }
  //result.y = 0.0f;
  if (glm::length(result) > 0) {
    //result = glm::normalize(result) * cameraSpeed * deltaTime;
    position_ += result;
  }
}

void Camera::UpdateYawPitchCallback(float xpos, float ypos) {
  static bool firstTime = true;
  static double lastX, lastY;
  if (firstTime) {
    lastX = xpos;
    lastY = ypos;
    firstTime = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw_ += xoffset;
  pitch_ += yoffset;

  if (pitch_ > 89.0f) {
    pitch_ = 89.0f;
  } else if (pitch_ < -89.0f) {
    pitch_ = -89.0f;
  }
}

void Camera::UpdateFOVCallback(float yoffset) {
  fov_ -= yoffset;
  fov_ = std::min(45.0f, std::max(fov_, 1.0f));
}

glm::vec3 Camera::GetDirection() const noexcept {
  glm::vec3 dir;
  dir.x = glm::cos(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
  dir.y = glm::sin(glm::radians(pitch_));
  dir.z = glm::sin(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
  return glm::normalize(dir);
}

}  // namespace over