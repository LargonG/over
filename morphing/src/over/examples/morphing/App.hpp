#pragma once

#include <over/core/Camera.hpp>
#include <over/core/Types.hpp>

#include <fmt/core.h>

namespace over {
constexpr uint32 WIDTH = 1980;
constexpr float32 ASPECT_RATIO = 16.0f / 9;
constexpr uint32 HEIGHT = WIDTH / ASPECT_RATIO;
constexpr const char* TITLE = "Morphing";

class App {

 public:
  App()
      : _context(),
        _window(WIDTH, HEIGHT, TITLE),
        _camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), 20.f,
                glm::radians(45.f), ASPECT_RATIO),
        _deltaTime(0) {
    if (!s_app) {
      s_app = this;
    }
  }

  static App& Instance() {
    assert(s_app != nullptr);
    return *s_app;
  }

  Context& GetContext() noexcept { return _context; }
  Camera& GetCamera() noexcept { return _camera; }
  Window& GetWindow() noexcept { return _window; }
  float32 GetDelta() const noexcept { return _deltaTime; }
  void SetDelta(float32 v) noexcept { _deltaTime = v; }
  bool IsCursorActive() const noexcept { return _cursorActive; }
  void SetCursorActive(bool newValue) const noexcept {
    _cursorActive = newValue;
  }

 private:
  Context _context;
  Window _window;
  Camera _camera;

  mutable float32 _deltaTime;
  mutable bool _cursorActive = false;

  static App* s_app;
};

App* App::s_app = nullptr;

void InputHandler() {
  static bool down = false;
  auto& app = App::Instance();
  auto& camera = app.GetCamera();
  auto* window = app.GetWindow().Get();
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !down) {
    down = true;
    camera.UpdateYawPitchCallback(0, 0, true);
    app.SetCursorActive(!app.IsCursorActive());
    glfwSetInputMode(
        window, GLFW_CURSOR,
        app.IsCursorActive() ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
  }

  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE && down) {
    down = false;
  }

  if (!app.IsCursorActive()) {
    camera.UpdatePositionCallback(window, app.GetDelta());
  }
}

static void CursorPositionHandler(GLFWwindow* window, float64 xpos,
                                  float64 ypos) {
  auto& app = App::Instance();
  if (!app.IsCursorActive()) {
    app.GetCamera().UpdateYawPitchCallback(static_cast<float32>(xpos),
                                           static_cast<float32>(ypos));
  }
}

static void ScrollHandler(GLFWwindow* window, float64 xoffset,
                          float64 yoffset) {
  auto& app = App::Instance();
  if (!app.IsCursorActive()) {
    App::Instance().GetCamera().UpdateFOVCallback(
        static_cast<float32>(yoffset));
  }
}
}  // namespace over
