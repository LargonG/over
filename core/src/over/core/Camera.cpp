#include <over/core/Camera.hpp>

#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace over {
Camera::Camera(glm::vec3 initialPosition, float initialYaw, float initialPitch,
               float initialFOV, glm::vec3 up, float aspectRatio, float near,
               float far)
    : _position(std::move(initialPosition)),
      _yaw(initialYaw),
      _pitch(initialPitch),
      _fov(initialFOV),
      _up(std::move(up)),
      _aspectRatio(aspectRatio),
      _near(near),
      _far(far) {}

void Camera::UpdatePositionCallback(GLFWwindow* window, float deltaTime) {
  const float cameraSpeed = 25.0f;
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
        deltaTime * cameraSpeed * glm::normalize(glm::cross(direction, _up));
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    result +=
        deltaTime * cameraSpeed * glm::normalize(glm::cross(direction, _up));
  }
  //result.y = 0.0f;
  if (glm::length(result) > 0) {
    //result = glm::normalize(result) * cameraSpeed * deltaTime;
    _position += result;
  }
}

void Camera::UpdateYawPitchCallback(float xpos, float ypos, bool flush) {
  static bool firstTime = true;
  static double lastX, lastY;
  if (firstTime) {
    lastX = xpos;
    lastY = ypos;
    firstTime = false;
  }

  if (flush) {
    firstTime = true;
    return;
  }

  float xoffset = static_cast<float>(xpos - lastX);
  float yoffset = static_cast<float>(lastY - ypos);
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  _yaw += xoffset;
  _pitch += yoffset;

  if (_pitch > 89.0f) {
    _pitch = 89.0f;
  } else if (_pitch < -89.0f) {
    _pitch = -89.0f;
  }
}

void Camera::UpdateFOVCallback(float yoffset) {
  _fov -= yoffset;
  _fov = std::min(45.0f, std::max(_fov, 1.0f));
}

glm::vec3 Camera::GetDirection() const noexcept {
  glm::vec3 dir;
  dir.x = glm::cos(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
  dir.y = glm::sin(glm::radians(_pitch));
  dir.z = glm::sin(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
  return glm::normalize(dir);
}

glm::mat4 Camera::GetView() const noexcept {
  return glm::lookAt(_position, _position + GetDirection(), _up);
}

glm::mat4 Camera::GetProjection() const noexcept {
  return glm::perspective(glm::radians(_fov), _aspectRatio, _near, _far);
}

}  // namespace over