#include <over/core/Camera.hpp>

#include <algorithm>

#include <fmt/core.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace over {

glm::vec3 Camera::up = glm::vec3(0, 1, 0);

Camera::Camera(glm::vec3 position, glm::vec3 rotation, float32 speed,
               float32 fov, float32 aspectRatio, float32 near, float32 far,
               float32 sensitivity)
    : _position(position),
      _rotation(rotation),
      _speed(speed),
      _fov(fov),
      _aspectRatio(aspectRatio),
      _near(near),
      _far(far),
      _sensitivity(sensitivity) {}

void Camera::UpdatePositionCallback(GLFWwindow* window, float deltaTime) {
  auto direction = GetDirection();
  glm::vec3 result(0.0f, 0.0f, 0.0f);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    result += direction;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    result -= direction;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    result -= glm::normalize(glm::cross(direction, up));
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    result += glm::normalize(glm::cross(direction, up));
  }
  //result.y = 0.0f;
  if (glm::length(result) > 0) {
    result = glm::normalize(result) * _speed * deltaTime;
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

  xoffset *= _sensitivity;
  yoffset *= _sensitivity;

  _rotation.x -= glm::radians(yoffset);
  _rotation.y += glm::radians(xoffset);

  if (_rotation.x > glm::radians(89.0f)) {
    _rotation.x = glm::radians(89.0f);
  } else if (_rotation.x < glm::radians(-89.0f)) {
    _rotation.x = glm::radians(-89.0f);
  }
}

void Camera::UpdateFOVCallback(float yoffset) {
  _fov -= yoffset;
  _fov = std::min(45.0f, std::max(_fov, 1.0f));
}

glm::vec3 Camera::GetDirection() const noexcept {
  glm::vec3 dir = glm::vec3(0, 0, -1);

  glm::vec3 right = glm::cross(dir, up);
  glm::vec3 backward = glm::cross(right, up);

  glm::mat4 m = glm::mat4(1);
  m = glm::translate(m, dir);
  m = glm::rotate(m, _rotation.x, right);
  m = glm::rotate(m, _rotation.y, up);
  m = glm::rotate(m, _rotation.z, backward);

  auto res = glm::normalize(glm::vec4(dir, 0) * m);
  return res;
}

glm::mat4 Camera::GetView() const noexcept {
  return glm::lookAt(_position, _position + GetDirection(), up);
}

glm::mat4 Camera::GetProjection() const noexcept {
  return glm::perspective(_fov, _aspectRatio, _near, _far);
}

}  // namespace over