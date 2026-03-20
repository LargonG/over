#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>

#include <string>

namespace over {

class Window {
 public:
  Window(uint32 width, uint32 height, const char* title,
         int32 swapInterval = 0);
  explicit Window(GLFWwindow* ptr);

  Window(const Window&) = delete;

  Window(Window&&) noexcept;
  Window& operator=(Window&&) noexcept;

  ~Window();

  void Activate();

  std::tuple<uint32, uint32> GetSize() const noexcept;

  uint32 GetWidth() const noexcept;
  void SetWidth(uint32 width);

  uint32 GetHeight() const noexcept;
  void SetHeight(uint32 height);

  float32 GetAspectRatio() const noexcept;

  const char* GetTitle() const noexcept;
  void SetTitle(const std::string& title);
  void SetTitle(const char* title);

  bool ShouldClose() const noexcept;
  void SetShouldClose(bool value) const noexcept;

  void SetSwapInterval(int32 swapInterval);
  void SwapBuffers() const noexcept;

  GLFWwindow* Get() { return _ptr; }

 private:
  int32 _swapInterval;
  GLFWwindow* _ptr;
};
}  // namespace over
