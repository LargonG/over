#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/window/Window.hpp>

#include <glm/glm.hpp>

namespace over {
// Context manages OpenGL settings (Version, Depth/Stencil Test, Face Culling e.t.c)
class Context {
 public:
  Context(int32 major = 3, int32 minor = 3);

  Context(const Context&) = delete;

  ~Context();

  void LoadOpenGL(Window& window);

  void Viewport(uint32 x, uint32 y, uint32 width, uint32 height);

  void SetClearColor(glm::vec4 color);
  void SetFaceCulling(bool value);
  void SetDepthTest(bool value);
  void SetStencilTest(bool value);

  void ClearAll();

  void ThrowErrors();
};
}  // namespace over
