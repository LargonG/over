#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cstdint>
#include <fstream>
#include <string>
#include "fmt/core.h"

#include "shader.hpp"

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
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

  float vertices[] = {
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,  // 0
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // 1
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // 2
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f,  // 3
  };

  uint32_t indices[] = {
      0, 1, 3,  // 0
      1, 2, 3,  // 1
  };

  int nAttrs;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttrs);
  fmt::println("maximum {}", nAttrs);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6,
                        reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  over::Shader shader("vertex.shader", "fragment.shader");
  shader.Compile();
  shader.Activate();

  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(window,
                                 [](GLFWwindow* window, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });
  glClearColor(0.2f, 0.3f, 0.3f, 1.f);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT);

    float timeValue = glfwGetTime();
    float greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
    glUniform4f(glGetUniformLocation(shader.GetProgram(), "globColor"), 0,
                greenValue, 0, 1.f);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}