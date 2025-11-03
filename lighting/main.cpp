#include <fmt/core.h>
#include <cstdint>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <camera.hpp>
#include <shader.hpp>
#include "mesh.hpp"

static over::Camera camera(glm::vec3(0, 0, 3), -90, 0, 45, glm::vec3(0, 1, 0));

static void ProcessInput(GLFWwindow* window, over::Camera& camera,
                         float deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
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
  fmt::println("Lighting");

  fmt::println("vertex sizeof: {}", sizeof(over::Vertex));
  fmt::println("vertex sizeof: {}", sizeof(over::Element));

  // Initialize GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  const float ASPECT_RATIO = 16.0 / 9.0;
  const int32_t INIT_WIDTH = 1920;
  const int32_t INIT_HEIGHT = INIT_WIDTH / ASPECT_RATIO;

  GLFWwindow* window =
      glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, "Lighting", nullptr, nullptr);
  if (!window) {
    fmt::println("Failed to create window");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fmt::println("Failed to initialize GLAD");
    glfwTerminate();
    return -1;
  }

  // Callbacks
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, CursorPositionHandler);
  glfwSetScrollCallback(window, ScrollHandler);

  // Init
  glm::vec3 lightPosition(0.0f, -1.0f, 0.5f);
  glm::vec3 lightColor(1, 1, 1);

  over::Shader shader("shaders/vertex.shader", "shaders/fragment.shader");
  shader.Compile();
  shader.Activate();
  glUniform3fv(glGetUniformLocation(shader.GetProgram(), "lightColor"), 1,
               glm::value_ptr(lightColor));
  glUniform3fv(glGetUniformLocation(shader.GetProgram(), "lightPosition"), 1,
               glm::value_ptr(lightPosition));

  over::Shader lightShader("shaders/vertex.shader",
                           "shaders/lightFragment.shader");
  lightShader.Compile();

  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;

  over::Mesh mesh = over::GenCubeMesh();
  for (size_t i = 0; i < mesh.Vertices().size(); i++) {
    mesh.Vertices()[i].color = glm::vec3(1.0f, 0.5f, 0.31f);
  }
  mesh.GenGPU();

  over::Mesh light = over::GenCubeMesh();
  for (size_t i = 0; i < light.Vertices().size(); i++) {
    light.Vertices()[i].color = lightColor;
  }
  light.GenGPU();

  std::vector<glm::vec3> positions = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  float deltaTime = 0.0f;

  // Main Loop
  while (!glfwWindowShouldClose(window)) {
    float startTime = glfwGetTime();

    ProcessInput(window, camera, deltaTime);

    shader.Activate();
    view = glm::lookAt(camera.GetPosition(),
                       camera.GetPosition() + camera.GetDirection(),
                       camera.GetUp());
    shader.SetMatrix4f("view", glm::value_ptr(view));
    glUniform3fv(glGetUniformLocation(shader.GetProgram(), "viewPosition"), 1,
                 glm::value_ptr(camera.GetPosition()));
    

    projection = glm::perspective(camera.FOV(), ASPECT_RATIO, 0.1f, 100.0f);
    shader.SetMatrix4f("projection", glm::value_ptr(projection));

    lightPosition =
        glm::vec3(glm::cos(startTime * glm::radians(90.0f)),
                  glm::sin(startTime * glm::radians(90.0f)),
          glm::sin(startTime * glm::radians(180.0f)) * 0.5);
    glm::vec3 lpos = view * glm::vec4(lightPosition, 1.0f);
    glUniform3fv(glGetUniformLocation(shader.GetProgram(), "lightPosition"), 1,
                 glm::value_ptr(lpos));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw
    mesh.Bind();
    for (size_t i = 0; i < positions.size(); i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, positions[i]);
      model = glm::rotate(model, startTime * glm::radians(i * 20.0f),
                          glm::vec3(0.5f, 1.0f, 0.0f));
      shader.SetMatrix4f("model", glm::value_ptr(model));

      glDrawElements(GL_TRIANGLES, mesh.Elements().size() * 3, GL_UNSIGNED_INT,
                     0);
    }

    mesh.Unbind();

    lightShader.Activate();
    lightShader.SetMatrix4f("view", glm::value_ptr(view));
    lightShader.SetMatrix4f("projection", glm::value_ptr(projection));
    light.Bind();
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPosition);
    model = glm::scale(model, glm::vec3(0.2f));
    lightShader.SetMatrix4f("model", glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, light.Elements().size() * 3, GL_UNSIGNED_INT,
                   0);

    light.Unbind();

    glfwSwapBuffers(window);
    glfwPollEvents();

    deltaTime = glfwGetTime() - startTime;
  }

  glfwTerminate();
  return 0;
}