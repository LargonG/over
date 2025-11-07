#include <array>
#include <string>
#include <vector>

#include <fmt/core.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include <camera.hpp>
#include <shader.hpp>

static const float ASPECT_RATIO = 16.0f / 9.0;
static const int WIDTH = 1080;
static const int HEIGHT = WIDTH / ASPECT_RATIO;
static const char* TITLE = "fractal";

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

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) &&
      !ImGui::IsAnyItemActive()) {
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

    glm::vec3 offset = -glm::vec3(xoffset * ASPECT_RATIO, yoffset, 0.f) * speed;
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

  glm::vec3 offset =
      glm::vec3(xoffset * ASPECT_RATIO, yoffset, 0.f) * (oldScale - scale);

  camera.GetPosition() += offset;
}

int main() {
  fmt::println("Fractal");

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(
      window,
      true);  // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();

  over::Shader shader("shaders/vertex.shader", "shaders/fragment.shader");
  shader.Compile();
  shader.Activate();

  
  shader.SetVec2f("offset", glm::vec2(0, 0));

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

  uint32_t fps = 0;
  uint32_t lastFps = 0;
  float deltaTime = 0;
  float lastTime = glfwGetTime();

  int k = 500;
  float gamma = 2.2f;

  while (!glfwWindowShouldClose(window)) {
    float startTime = glfwGetTime();
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Parameters", nullptr, ImGuiWindowFlags_MenuBar);

    if (startTime - lastTime > 1) {
      lastFps = fps;
      fps = 0;
      lastTime = startTime;
    }
    ImGui::Text("fps: %d", lastFps);

    ImGui::SliderInt("Iterations", &k, 1, 1000);
    ImGui::InputInt("Manual Iterations", &k, 1, 2000);
    shader.SetInt("k", k);

    ImGui::SliderFloat("Gamma", &gamma, 0.1, 5.f);
    ImGui::InputFloat("Manual Gamma", &gamma);
    shader.SetFloat("gamma", gamma);

    ImGui::End();

    // input
    ProcessInput(window);

    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);

    model = glm::mat4(1.f);
    view = glm::lookAt(camera.GetPosition(),
                       camera.GetPosition() + camera.GetDirection(),
                       camera.GetUp());
    projection = glm::ortho(-scale * ASPECT_RATIO, scale * ASPECT_RATIO, -scale,
                            scale, -100.f, +100.f);

    shader.SetMatrix4f("model", glm::value_ptr(model));
    shader.SetMatrix4f("view", glm::value_ptr(view));
    shader.SetMatrix4f("projection", glm::value_ptr(projection));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);

    deltaTime = glfwGetTime() - startTime;
    fps++;
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);

  glfwTerminate();

  return 0;
}