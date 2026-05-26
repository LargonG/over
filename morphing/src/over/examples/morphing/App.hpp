#pragma once

#include <over/core/Camera.hpp>
#include <over/core/Types.hpp>
#include <over/core/window/Context.hpp>
#include <over/core/window/Window.hpp>

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

void InputHandler();
void ScrollHandler(GLFWwindow* window, float64 xoffset, float64 yoffset);
void CursorPositionHandler(GLFWwindow* window, float64 xpos, float64 ypos);

}  // namespace over
