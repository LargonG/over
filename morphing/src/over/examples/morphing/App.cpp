#include <over/examples/morphing/App.hpp>

namespace over {

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

void CursorPositionHandler(GLFWwindow* window, float64 xpos, float64 ypos) {
  auto& app = App::Instance();
  if (!app.IsCursorActive()) {
    app.GetCamera().UpdateYawPitchCallback(static_cast<float32>(xpos),
                                           static_cast<float32>(ypos));
  }
}

void ScrollHandler(GLFWwindow* window, float64 xoffset, float64 yoffset) {
  auto& app = App::Instance();
  if (!app.IsCursorActive()) {
    App::Instance().GetCamera().UpdateFOVCallback(
        static_cast<float32>(yoffset));
  }
}

}  // namespace over
