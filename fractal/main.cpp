#include <array>
#include <string>
#include <vector>

#include <fmt/core.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <camera.hpp>
#include <shader.hpp>

static over::Camera camera(glm::vec3(0.f, 0.f, -1.f), -90.f, 0.f, 0,
                           glm::vec3(0, 1, 0));
static float scale = 1.0f;

static void ProcessInput(GLFWwindow* window) {
  static bool mousePressed;
  static double lastPosx;
  static double lastPosy;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    xpos /= width;
    ypos /= -height;

    if (!mousePressed) {
      lastPosx = xpos;
      lastPosy = ypos;
    }
    mousePressed = true;

    float speed = 2 * scale;
    float xoffset = static_cast<float>(xpos - lastPosx);
    float yoffset = static_cast<float>(ypos - lastPosy);

    glm::vec3 offset = -glm::vec3(xoffset, yoffset, 0.f) * speed;
    camera.GetPosition() += offset;

    lastPosx = xpos;
    lastPosy = ypos;
  } else {
    mousePressed = false;
  }
}

static void ScrollHandler(GLFWwindow* window, double _, double yscroll) {
  const float speed = 0.25f;
  const float lowerBorder = 5e-6f;
  const float upperBorder = 100.f;

  const float oldScale = scale;

  scale += -1.f * yscroll * scale * speed;
  if (scale <= lowerBorder || scale >= upperBorder) {
    scale = std::max(lowerBorder, std::min(scale, upperBorder));
  }

  // move camera to cursor
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  xpos /= width;
  ypos /= height;

  float xoffset = 2 * (xpos - 0.5f);
  float yoffset = -2 * (ypos - 0.5f);

  glm::vec3 offset = glm::vec3(xoffset, yoffset, 0.f) * (oldScale - scale);

  camera.GetPosition() += offset;
}

int main() {
  fmt::println("Fractal");

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  const float ASPECT_RATIO = 16.0f / 9.0;
  const int WIDTH = 1080;
  const int HEIGHT = WIDTH / ASPECT_RATIO;
  const char* TITLE = "fractal";

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
  if (!window) {
    const char* err = "Window creation failed";
    fmt::println(err);
    glfwTerminate();
    throw fmt::system_error(-1, err);
  }

  glfwMakeContextCurrent(window);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fmt::println("Failed to initialize GLAD");
    glfwTerminate();
    return -1;
  }

  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetWindowSizeCallback(window,
                            [](GLFWwindow* window, int width, int height) {
                              glViewport(0, 0, width, height);
                            });
  glfwSetScrollCallback(window, ScrollHandler);

  over::Shader shader("shaders/vertex.shader", "shaders/fragment.shader");
  shader.Compile();
  shader.Activate();

  shader.SetInt("k", 500);
  shader.SetVec2f("offset", glm::vec2(-0.75f, 0));

  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;

  float mi = -10.f;
  float ma = 10.f;

  std::vector<float> vertices = {
      mi, mi, 0.f, 1.f, 0.f, 0.f,  // 0
      ma, mi, 0.f, 0.f, 1.f, 0.f,  // 1
      ma, ma, 0.f, 0.f, 0.f, 1.f,  // 2
      mi, ma, 0.f, 0.f, 1.f, 0.f,  // 3
  };

  std::vector<uint32_t> elements = {
      2, 1, 0,  // 0
      0, 2, 3,  // 1
  };

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6,
                        reinterpret_cast<void*>(sizeof(float) * 3));
  glEnableVertexAttribArray(1);

  GLuint ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(uint32_t),
               elements.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glClearColor(0.f, 0.f, 0.f, 1.0f);

  float f;

  //ImGui::Text("Hello, world %d", 123);
  //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

  while (!glfwWindowShouldClose(window)) {
    // input
    ProcessInput(window);

    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);

    model = glm::mat4(1.f);
    view = glm::lookAt(camera.GetPosition(),
                       camera.GetPosition() + camera.GetDirection(),
                       camera.GetUp());
    projection = glm::ortho(-scale, scale, -scale, scale, -100.f, +100.f);

    shader.SetMatrix4f("model", glm::value_ptr(model));
    shader.SetMatrix4f("view", glm::value_ptr(view));
    shader.SetMatrix4f("projection", glm::value_ptr(projection));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}