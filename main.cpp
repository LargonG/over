#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cstdint>
#include <fstream>
#include <string>
#include "fmt/core.h"

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

std::string getShader(std::string filename) {
  std::ifstream file(filename);
  if (file.fail()) {
    fmt::system_error(-2, "Cannot open file: {}", filename);
  }

  std::string str;
  std::string result;
  while (std::getline(file, str)) {
    if (file.fail()) {
      fmt::system_error(-2, "Cannot read from file: {}", filename);
    }
    result += str;
    result.push_back('\n');
  }

  return result;
}

void isCompiled(GLuint shader) {
  int32_t success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
    throw fmt::system_error(-1, "vertex shader compilation error: {}", infoLog);
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
      0.5f,  0.5f,  0.0f,  // top right
      0.5f,  -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f   // top left 
  };

  uint32_t indices[] = {
      0, 1, 3,  // 0
      1, 2, 3,  // 1
  };

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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  std::string vertexShaderSourceStr = getShader("vertex.shader");
  const char* vertexShaderSource = vertexShaderSourceStr.c_str();
  fmt::println("vertex shader:\n{}", vertexShaderSourceStr);

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);
  isCompiled(vertexShader);

  std::string fragmentShaderSourceStr = getShader("fragment.shader");
  const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();
  fmt::println("fragment shader:\n{}", fragmentShaderSourceStr);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);
  isCompiled(fragmentShader);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
    throw fmt::system_error(-1, "cannot link shader program: {}", infoLog);
  }

  glUseProgram(shaderProgram);

  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(window,
                                 [](GLFWwindow* window, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });
  glClearColor(0.2f, 0.3f, 0.3f, 1.f);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}