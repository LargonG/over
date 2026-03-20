#include <over/core/window/Window.hpp>

#include <cassert>
#include <stdexcept>
#include <utility>

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>

namespace over {
Window::Window(uint32 width, uint32 height, const char* title,
               int32 swapInterval) {
  _ptr = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height),
                          title, nullptr, nullptr);
  if (_ptr == nullptr) {
    throw std::runtime_error("GLFW failed to create window");
  }

  SetSwapInterval(swapInterval);
}

Window::Window(GLFWwindow* ptr) : _ptr(ptr), _swapInterval(0) {}

Window::Window(Window&& other) noexcept : Window(nullptr) {
  *this = std::move(other);
}

Window& Window::operator=(Window&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  if (_ptr != nullptr) {
    glfwDestroyWindow(_ptr);
  }
  _ptr = std::exchange(other._ptr, nullptr);

  return *this;
}

Window::~Window() {
  if (_ptr != nullptr) {
    glfwDestroyWindow(_ptr);
  }
}

void Window::Activate() {
  assert(_ptr != nullptr);
  glfwMakeContextCurrent(_ptr);
}

std::tuple<uint32, uint32> Window::GetSize() const noexcept {
  int width, height;
  glfwGetWindowSize(_ptr, &width, &height);
  return {static_cast<uint32>(width), static_cast<uint32>(height)};
}

uint32 Window::GetWidth() const noexcept {
  auto [width, _] = GetSize();
  return width;
}

void Window::SetWidth(uint32 width) {
  glfwSetWindowSize(_ptr, static_cast<int>(width),
                    static_cast<int>(GetHeight()));
}

uint32 Window::GetHeight() const noexcept {
  auto [_, height] = GetSize();
  return height;
}

void Window::SetHeight(uint32 height) {
  glfwSetWindowSize(_ptr, static_cast<int>(GetWidth()),
                    static_cast<int>(height));
}

float32 Window::GetAspectRatio() const noexcept {
  auto [width, height] = GetSize();
  return width * 1.f / height;
}

const char* Window::GetTitle() const noexcept {
  return glfwGetWindowTitle(_ptr);
}

void Window::SetTitle(const std::string& title) {
  glfwSetWindowTitle(_ptr, title.c_str());
}

void Window::SetTitle(const char* title) {
  glfwSetWindowTitle(_ptr, title);
}

bool Window::ShouldClose() const noexcept {
  return glfwWindowShouldClose(_ptr);
}

void Window::SetShouldClose(bool value) const noexcept {
  glfwSetWindowShouldClose(_ptr, value);
}

void Window::SetSwapInterval(int32 swapInterval) {
  _swapInterval = swapInterval;
}

void Window::SwapBuffers() const noexcept {
  glfwSwapInterval(_swapInterval);
  glfwSwapBuffers(_ptr);
}
}  // namespace over
