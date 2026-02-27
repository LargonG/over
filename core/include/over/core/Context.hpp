#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>

namespace over {
class Context {
 public:
  Context(int32 major = 3, int32 minor = 3);

  ~Context() noexcept;

  Context(const Context&) = delete;

  GLFWwindow* CreateWindow(uint32 width, uint32 height, const char* title);
  void LoadOpenGL();

  GLFWwindow* GetWindow() const noexcept { return _window; }

 private:
  GLFWwindow* _window;
};
}  // namespace over
