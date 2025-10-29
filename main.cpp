#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cstdint>
#include <fstream>
#include <string>
#include <utility>
#include "fmt/core.h"

#include "camera.hpp"
#include "shader.hpp"

over::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), -90.0f, 0.0f, 45.0f,
                    glm::vec3(0.0f, 1.0f, 0.0f));
float deltaTime;

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  camera.UpdatePositionCallback(window, deltaTime);
}

void cursorPositionUpdateCallback(GLFWwindow* window, double xpos,
                                  double ypos) {
  camera.UpdateYawPitchCallback(xpos, ypos);
}

void scroolCallback(GLFWwindow* window, double xoffset, double yoffset) {
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

glm::mat4 LookAt(glm::vec3 position, glm::vec3 target, glm::vec3 globalUp) {
  auto backward = glm::normalize(position - target);
  auto right = glm::normalize(glm::cross(globalUp, backward));
  auto up = glm::normalize(glm::cross(backward, right));

  glm::mat4 result(glm::vec4(right, 0.0f), glm::vec4(up, 0.0f),
                   glm::vec4(backward, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  result = glm::transpose(result);
  result *= glm::translate(glm::mat4(1.0f), -position);
  
  return result;
}

int main() {
  // this code paragraph is for tutorial purposes
  glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 1.0f));
  vec = trans * vec;
  fmt::println("{}, {}, {}", vec.x, vec.y, vec.z);

  // this too
  trans = glm::mat4(1.0f);
  trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
  vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  vec = trans * vec;
  fmt::println("{}, {}, {}", vec.x, vec.y, vec.z);

  fmt::println("Hello, opengl app!");

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  constexpr int32_t WIDTH = 1920;
  constexpr int32_t HEIGHT = 1080;

  GLFWwindow* window = glfwCreateWindow(
      WIDTH, HEIGHT, "Hello, opengl, finally!", nullptr, nullptr);
  if (!window) {
    fmt::println("Failed to create glfw window");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fmt::println("Failed to initialize GLAD");
    return -1;
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, cursorPositionUpdateCallback);
  glfwSetScrollCallback(window, scroolCallback);

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 0
      0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,  // 1
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  // 2
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  // 3
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  // 4
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 5

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  // 6
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  // 7
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // 8
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // 9
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,  // 10
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  // 11

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  // 12
      -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,  // 13
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // 14
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // 15
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  // 16
      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  // 17

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // 18
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  // 19
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  // 20
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  // 21
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f,  // 22
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // 23

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // 24
      0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,  // 25
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  // 26
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  // 27
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  // 28
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // 29

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  // 30
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  // 31
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // 32
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // 33
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,  // 34
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f   // 35
  };

  //uint32_t indices[] = {
  //    0, 1, 3,  // 0
  //    1, 2, 3,  // 1
  //};

  int nAttrs;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttrs);
  fmt::println("maximum {}", nAttrs);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //GLuint ibo;
  //glGenBuffers(1, &ibo);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //             GL_STATIC_DRAW);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);
  glEnableVertexAttribArray(0);

  /*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
                        reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
                        reinterpret_cast<void*>(6 * sizeof(float)));
  glEnableVertexAttribArray(2);*/

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
                        reinterpret_cast<void*>(sizeof(float) * 3));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  glm::mat4 model;

  /*glm::mat4 view = glm::mat4(1.0f);
  view = glm::rotate(view, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::translate(view, glm::vec3(0.0f, -30.0f, -30.0f));*/

  glm::mat4 view;
  glm::mat4 proj;

  GLuint texture1 = loadTexture("textures/container.jpg");
  GLuint texture2 = loadTexture("textures/awesomeface.png", true);

  over::Shader shader("vertex.shader", "fragment.shader");
  shader.Compile();
  shader.Activate();

  shader.SetInt("texture1", 0);
  shader.SetInt("texture2", 1);

  shader.SetMatrix4f("transform", glm::value_ptr(trans));

  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(window,
                                 [](GLFWwindow* window, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });
  glClearColor(0.2f, 0.3f, 0.3f, 1.f);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glEnable(GL_DEPTH_TEST);

  float lastUpdate = glfwGetTime();
  int32_t fps = 0;

  while (!glfwWindowShouldClose(window)) {
    float start = glfwGetTime();
    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float timeValue = glfwGetTime();

    float greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
    glUniform4f(shader.GetUniformLocation("globColor"), 0, greenValue, 0, 1.f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(vao);

    /*view = glm::lookAt(camera.GetPosition(),
                       camera.GetPosition() + camera.GetDirection(),
                       camera.GetUp());*/
    view = LookAt(camera.GetPosition(),
                  camera.GetPosition() + camera.GetDirection(), camera.GetUp());
    shader.SetMatrix4f("view", glm::value_ptr(view));

    proj = glm::perspective(glm::radians(camera.FOV()), WIDTH * 1.0f / HEIGHT,
                            0.1f, 100.0f);
    shader.SetMatrix4f("projection", glm::value_ptr(proj));

    // local logic
    for (std::size_t i = 0; i < 10; i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::rotate(model, timeValue * glm::radians(i * 20.0f),
                          glm::vec3(0.5f, 1.0f, 0.0f));
      shader.SetMatrix4f("model", glm::value_ptr(model));

      trans = glm::mat4(1.0f);
      trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
      trans = glm::rotate(trans, timeValue, glm::vec3(0.0f, 0.0f, 1.0f));

      shader.SetMatrix4f("transform", glm::value_ptr(trans));

      glDrawArrays(GL_TRIANGLES, 0, 36);
      //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      trans = glm::mat4(1.0f);
      trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
      trans = glm::scale(trans, glm::vec3(glm::abs(glm::sin(timeValue))));

      shader.SetMatrix4f("transform", glm::value_ptr(trans));

      glDrawArrays(GL_TRIANGLES, 0, 36);
      //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();

    fps++;
    if (glfwGetTime() - lastUpdate > 1.0f) {
      lastUpdate = glfwGetTime();
      fmt::println("fps: {}", fps);
      fps = 0;
    }

    deltaTime = glfwGetTime() - start;
  }

  glfwTerminate();

  return 0;
}