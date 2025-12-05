#include <cstdint>
#include <string>

#include <fmt/core.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "camera.hpp"
#include "mmodel.hpp"
#include "shader.hpp"

using Window = GLFWwindow;

static over::Camera camera(glm::vec3(0, 0, 3), -90, 0, 45, glm::vec3(0, 1, 0));
constexpr float ASPECT_RATIO = 16.0f / 9;
constexpr uint32_t INIT_WIDTH = 1920;
constexpr uint32_t INIT_HEIGHT =
    static_cast<uint32_t>(INIT_WIDTH / ASPECT_RATIO);
constexpr char* TITLE = "Models";

void InputHandler(Window* window, float deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  camera.UpdatePositionCallback(window, deltaTime);
}

static void CursorPositionHandler(GLFWwindow* window, double xpos,
                                  double ypos) {
  camera.UpdateYawPitchCallback(xpos, ypos);
}

static void ScrollHandler(GLFWwindow* window, double xoffset, double yoffset) {
  camera.UpdateFOVCallback(yoffset);
}

int main() {
  fmt::println(TITLE);

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  Window* window =
      glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, TITLE, nullptr, nullptr);
  if (!window) {
    fmt::println("Error: cannot create window");

    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    fmt::println("Error: cannot link glad");

    glfwTerminate();
    return -1;
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, CursorPositionHandler);
  glfwSetScrollCallback(window, ScrollHandler);

  glfwSetFramebufferSizeCallback(window,
                                 [](GLFWwindow* window, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;

  glm::vec3 globalLightDirection(-0.0f, -1.0f, -1.f);

  glm::vec3 globalLightColor(1.f, 1.f, 1.f);

  over::Shader shader("shaders/vertex.shader", "shaders/fragment.shader");
  shader.Compile();
  shader.Activate();

  over::Shader outlineShader("shaders/stencil_vertex.shader",
                             "shaders/stencil_fragment.shader");
  outlineShader.Compile();

  over::MModel entity("resources/backpack/backpack.obj");

  float deltaTime = 0.0f;

  glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    float startTime = glfwGetTime();

    shader.Activate();

    view = glm::lookAt(camera.GetPosition(),
                       camera.GetPosition() + camera.GetDirection(),
                       camera.GetUp());
    shader.SetMatrix4f("view", glm::value_ptr(view));

    projection = glm::perspective(glm::radians(camera.FOV()), ASPECT_RATIO,
                                  0.1f, 100.0f);
    shader.SetMatrix4f("projection", glm::value_ptr(projection));

    glm::vec3 globalDir = view * glm::vec4(globalLightDirection, 0.f);
    shader.SetVec3f("light.direction", globalDir);
    shader.SetVec3f("light.ambient", globalLightColor * 0.2f * 0.75f);
    shader.SetVec3f("light.diffuse", globalLightColor * 0.75f);
    shader.SetVec3f("light.specular", globalLightColor * 1.0f);

    InputHandler(window, deltaTime);

    glEnable(GL_DEPTH_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilMask(0xff);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    model = glm::mat4(1.0f);
    shader.SetMatrix4f("model", glm::value_ptr(model));

    shader.SetFloat("material.shininess", 32.0f);

    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilMask(0xff);

    entity.Draw(shader);

    glStencilFunc(GL_NOTEQUAL, 1, 0xff);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    outlineShader.Activate();
    outlineShader.SetMatrix4f("view", glm::value_ptr(view));
    outlineShader.SetMatrix4f("projection", glm::value_ptr(projection));

    model = glm::scale(model, glm::vec3(1.02f));
    outlineShader.SetMatrix4f("model", glm::value_ptr(model));
    entity.Draw(outlineShader);

    glfwSwapBuffers(window);

    deltaTime = glfwGetTime() - startTime;
  }

  glfwTerminate();

  return 0;
}
