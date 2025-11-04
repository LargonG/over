#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

GLuint loadTexture(std::string filename, bool png = false) {
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  stbi_set_flip_vertically_on_load(true);

  int imgWidth, imgHeight, nrChannels;
  unsigned char* imgData =
      stbi_load(filename.c_str(), &imgWidth, &imgHeight, &nrChannels, 0);
  if (imgData == nullptr) {
    fmt::println("cannot load file: {}", filename);
    throw fmt::system_error(-1, "cannot load file");
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0,
               png ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imgData);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(imgData);

  glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}

int main() {
  fmt::println("Lighting");

  fmt::println("vertex sizeof: {}", sizeof(over::Vertex));
  fmt::println("element sizeof: {}", sizeof(over::Element));

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

  glfwSetFramebufferSizeCallback(window,
                                 [](GLFWwindow* window, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });

  // Init
  GLuint containerTexture = loadTexture("textures/container2.png", true);
  GLuint containerSpecularTexture =
      loadTexture("textures/container2_specular.png", true);

  glm::vec3 lightPosition(0.0f, -1.0f, 0.5f);
  glm::vec3 lightColor(1, 1, 1);

  over::Shader shader("shaders/vertex.shader", "shaders/fragment.shader");
  shader.Compile();
  shader.Activate();

  shader.SetInt("material.diffuse", 0);
  shader.SetInt("material.specular", 1);
  shader.SetFloat("material.shininess", 64.0f);

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

  glClearColor(0.05f, 0.05f, 0.06f, 1.0f);
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
    shader.SetVec3f("viewPosition", camera.GetPosition());

    projection = glm::perspective(camera.FOV(), ASPECT_RATIO, 0.1f, 100.0f);
    shader.SetMatrix4f("projection", glm::value_ptr(projection));

    lightPosition = glm::vec3(glm::cos(startTime * glm::radians(2.0f)),
                              glm::sin(startTime * glm::radians(2.0f)),
                              glm::sin(startTime * glm::radians(20.0f)) * 0.5);
    glm::vec3 lpos = view * glm::vec4(lightPosition, 1.0f);
    shader.SetVec3f("light.position", lpos);

    float freeze = 0.1f;
    lightColor.x = glm::abs(glm::sin(startTime * 2.0f * freeze) * 0.25f + 0.5f);
    lightColor.y = glm::abs(glm::sin(startTime * 0.7f * freeze) * 0.25f + 0.5f);
    lightColor.z = glm::abs(glm::sin(startTime * 1.3f * freeze) * 0.25f + 0.5f);

    shader.SetVec3f("light.ambient", lightColor * 0.2f * 0.75f);
    shader.SetVec3f("light.diffuse", lightColor * 0.75f);
    shader.SetVec3f("light.specular", lightColor);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, containerTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, containerSpecularTexture);

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
    lightShader.SetVec3f("lightColor", lightColor);
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

  glDeleteTextures(1, &containerTexture);
  glDeleteTextures(1, &containerSpecularTexture);

  glfwTerminate();
  return 0;
}