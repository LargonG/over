#pragma once

#include <string>

#include <over/core/Types.hpp>
#include <over/core/window/Context.hpp>
#include <over/core/window/Window.hpp>
#include <over/engine/Input.hpp>

namespace over {

class App {
 public:
  static App& Instance();

  App(std::string name);

  void Run(int32 width, int32 height, int32 swapInterval = 0);

  virtual void Init() = 0;
  virtual void Update(float32 dt) = 0;

  void PrintName() const;

  Context& GetContext() noexcept { return _ctx; }
  Window& GetWindow() noexcept { return _window; }
  Input& GetInput() noexcept { return _input; }

 protected:
  std::string _name;

  Context _ctx;
  Window _window;
  Input _input;

  int32 _fps;

 private:
  static App* s_App;
};

}  // namespace over
