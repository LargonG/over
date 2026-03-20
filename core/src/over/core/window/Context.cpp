#include <over/core/window/Context.hpp>

#include <cassert>
#include <stdexcept>

#include <fmt/core.h>

namespace over {
Context::Context(int32 major, int32 minor) {
  auto err = glfwInit();
  if (err != GLFW_TRUE) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Context::~Context() {
  glfwTerminate();
}

void Context::LoadOpenGL(Window& window) {
  window.Activate();
  if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
    throw std::runtime_error("Failed to initialize OpenGL context\n");
  }
}

void Context::Viewport(uint32 x, uint32 y, uint32 width, uint32 height) {
  glViewport(static_cast<GLint>(x), static_cast<GLint>(y),
             static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

static void Toggle(GLenum name, bool value) {
  if (value) {
    glEnable(name);
  } else {
    glDisable(name);
  }
}

void Context::SetClearColor(glm::vec4 color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void Context::SetFaceCulling(bool value) {
  Toggle(GL_CULL_FACE, value);
}

void Context::SetDepthTest(bool value) {
  Toggle(GL_DEPTH_TEST, value);
}

void Context::SetStencilTest(bool value) {
  Toggle(GL_STENCIL_TEST, value);
}

static GLenum GetBit(GLenum name, GLbitfield bit) {
  if (glIsEnabled(name)) {
    return bit;
  }
  return 0;
}

void Context::ClearAll() {
  glClear(GL_COLOR_BUFFER_BIT | GetBit(GL_DEPTH_TEST, GL_DEPTH_BUFFER_BIT) |
          GetBit(GL_STENCIL_TEST, GL_STENCIL_BUFFER_BIT));
}

void Context::ThrowErrors() {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    throw std::runtime_error(fmt::format("OpenGL error: {}", err));
  }
}
}  // namespace over
