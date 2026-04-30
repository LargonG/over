#include <over/engine/Input.hpp>

#include <over/engine/App.hpp>

#include <GLFW/glfw3.h>

namespace over {

Input& Input::Instance() noexcept {
  return App::Instance().GetInput();
}

void Input::Poll() const noexcept {
  glfwPollEvents();
}

void Input::Flush() const noexcept {
  _scrollOffsetX = 0.f;
  _scrollOffsetY = 0.f;
}

static GLFWwindow* GetWindow() {
  auto* result = glfwGetCurrentContext();
  return result;
}

void Input::SetCursor(bool enabled) {
  auto* window = GetWindow();
  glfwSetInputMode(window, GLFW_CURSOR,
                   enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

std::tuple<float32, float32> Input::GetCursorPosition() const noexcept {
  auto* window = GetWindow();
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  return {static_cast<float32>(xpos), static_cast<float32>(ypos)};
}

std::tuple<float32, float32> Input::GetCursorScroll() const noexcept {
  return {_scrollOffsetX, _scrollOffsetY};
}

bool Input::IsPressed(Key key) const noexcept {
  auto* window = GetWindow();
  return glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS;
}

bool Input::IsReleased(Key key) const noexcept {
  auto* window = GetWindow();
  return glfwGetKey(window, static_cast<int>(key)) == GLFW_RELEASE;
}

void Input::UpdateScroll(float32 xoffset, float32 yoffset) {
  _scrollOffsetX = xoffset;
  _scrollOffsetY = yoffset;
}

void Input::Init() {
  SetCursor(false);

  glfwSetScrollCallback(
      GetWindow(), [](GLFWwindow* window, double xoffset, double yoffset) {
        Input::Instance().UpdateScroll(static_cast<float32>(xoffset),
                                          static_cast<float32>(yoffset));
      });
}

}  // namespace over
