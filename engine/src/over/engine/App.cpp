#include <over/engine/App.hpp>

#include <fmt/core.h>
#include <over/utils/Interval.hpp>

namespace over {

App* App::s_App = nullptr;

App& App::Instance() {
  return *s_App;
}

App::App(std::string name)
    : _name(std::move(name)), _ctx(), _window(nullptr), _input(), _fps(0) {
  assert(s_App == nullptr);
  s_App = this;
}

void App::Run(int32 width, int32 height, int32 swapInterval) {
  _window = Window(static_cast<uint32>(width), static_cast<uint32>(height),
                   _name.c_str(), swapInterval);
  _ctx.LoadOpenGL(_window);

  float32 startTime = 0;         // loop has not started yet
  float32 deltaTime = 1 / 60.f;  // 60 frames per second

  int32 fpsCounter = 0;
  Interval fpsInterval(static_cast<float32>(glfwGetTime()), 1.f);

  Init();

  while (!_window.ShouldClose()) {

    startTime = glfwGetTime();

    _input.Poll();
    _ctx.ClearAll();

    Update(deltaTime);

    _ctx.ThrowErrors();
    _input.Flush();
    _window.SwapBuffers();

    float32 endTime = glfwGetTime();

    deltaTime = endTime - startTime;

    fpsCounter++;

    if (fpsInterval.Check(endTime)) {
      _fps = fpsCounter;
      fpsCounter = 0;
    }
  }
}

void App::PrintName() const {
  fmt::println(_name);
}
}  // namespace over
