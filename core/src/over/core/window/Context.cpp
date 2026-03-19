#include <over/core/window/Context.hpp>

#include <stdexcept>

namespace over {

Context::Context(int32 major, int32 minor) : _window(nullptr) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* Context::CreateWindow(uint32 width, uint32 height,
                                  const char* title) {
  _window =
      glfwCreateWindow(static_cast<int32>(width), static_cast<int32>(height),
                       title, nullptr, nullptr);
  if (_window == nullptr) {
    throw std::runtime_error("Cannot create window");
  }
  glfwMakeContextCurrent(_window);

  return _window;
}

void Context::LoadOpenGL() {
  if (glfwGetCurrentContext() != _window) {
    throw std::runtime_error(
        "Current context should be the same, as this context");
  }

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    throw std::runtime_error("Cannot load OpenGL");
  }
}

Context::~Context() noexcept {
  if (_window != nullptr) {
    glfwDestroyWindow(_window);
    _window = nullptr;
  }
  glfwTerminate();
}

}  // namespace over
