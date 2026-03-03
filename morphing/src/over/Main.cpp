#include <over/core/Camera.hpp>
#include <over/core/Context.hpp>
#include <over/core/Model.hpp>
#include <over/core/Shader.hpp>
#include <over/core/Types.hpp>
#include <over/examples/morphing/App.hpp>
#include <over/examples/morphing/DirectionalLight.hpp>
#include <over/examples/morphing/SpotLight.hpp>

#include <glm/glm.hpp>

#include <fmt/core.h>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

namespace over {

static float32 SphereCoords(float32 posa, float32 posb2, float32 posd2) {
  return posa * std::sqrt(1 - posb2 / 2.0 - posd2 / 2.0 + posb2 * posd2 / 3);
}

static glm::vec3 Interpolate(float32 coef, glm::vec3 a, glm::vec3 b) {
  return a * (1 - coef) + coef * b;
}

static bool Check(const std::vector<Vertex>& v) {
  for (usize i = 0; i < v.size(); i++) {
    for (usize j = i + 1; j < v.size(); j++) {
      if (v[i].position == v[j].position) {
        fmt::println("{} {}, {} {} {}", i, j, v[i].position.x, v[i].position.y,
                     v[i].position.z);
        throw std::runtime_error("should not be true");
      }
    }
  }
}

static void Morph(const std::vector<Vertex>& from,
                  const std::vector<Vertex>& to,
                  const std::vector<Element>& elements,
                  std::vector<Vertex>& result, float32 coef) {
  // from.size() == to.size() == result.size()

  for (usize i = 0; i < result.size(); i++) {
    auto& vertex = result[i];

    vertex.position = Interpolate(coef, from[i].position, to[i].position);
    vertex.normal = glm::vec3(0);
  }

  for (const Element& element : elements) {
    auto a = result[element.b].position - result[element.a].position;
    auto b = result[element.c].position - result[element.a].position;

    auto normal = glm::cross(a, b);

    result[element.a].normal += normal;
    result[element.b].normal += normal;
    result[element.c].normal += normal;
  }

  for (usize i = 0; i < result.size(); i++) {
    result[i].normal = glm::normalize(result[i].normal);
  }
}

static std::vector<Vertex> GetSphereVerticies(const std::vector<Vertex>& cube) {
  std::vector<Vertex> result(cube.size(), Vertex());
  for (usize i = 0; i < cube.size(); i++) {
    auto pos = cube[i].position;

    auto posx2 = pos.x * pos.x;
    auto posy2 = pos.y * pos.y;
    auto posz2 = pos.z * pos.z;

    result[i].position.x = SphereCoords(pos.x, posy2, posz2);
    result[i].position.y = SphereCoords(pos.y, posx2, posz2);
    result[i].position.z = SphereCoords(pos.z, posx2, posy2);

    // do not need normals & texCoords
  }
  return result;
}

void Run() {
  fmt::println(TITLE);

  App& app = App::Instance();
  auto& ctx = app.GetContext();
  auto& camera = app.GetCamera();

  auto* window = ctx.CreateWindow(WIDTH, HEIGHT, TITLE);
  ctx.LoadOpenGL();

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, CursorPositionHandler);
  glfwSetScrollCallback(window, ScrollHandler);

  glfwSetFramebufferSizeCallback(window,
                                 [](GLFWwindow* window, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });

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

  Shader shader("shaders/vertex.shader", "shaders/fragment.shader");
  shader.Activate();

  Model model("resources/cube/cube.glb");
  auto& modelMesh = model.GetMeshes().back();

  std::vector<Vertex> from = model.GetMeshes().back().GetVBO().GetVerticies();
  std::vector<Vertex> to = GetSphereVerticies(from);

  Light dirLightColor(glm::vec3(0.25, 0.25, 0.25), glm::vec3(0.5, 0.5, 0.5),
                      glm::vec3(1, 1, 1));
  Light spotLightColor(glm::vec3(0), glm::vec3(0.5, 0.5, 0.5),
                       glm::vec3(1, 1, 1));

  float32 cutOffDeg = 12.f;
  float32 outerCutOffDeg = 25.f;

  DirectionalLight dirLight(glm::vec3(1, -1, 1), dirLightColor);

  SpotLight spotLight(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1),
                      glm::cos(glm::radians(cutOffDeg)),
                      glm::cos(glm::radians(outerCutOffDeg)), spotLightColor);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.f, 0.f, 0.f, 1.0);

  uint32 fps = 0;
  uint32 lastFps = 0;
  float32 lastTime = glfwGetTime();
  float32 morphing = 0.5f;

  Morph(from, to, modelMesh.GetIBO().GetElements(),
        modelMesh.GetVBO().GetVerticies(), morphing);
  model.GetMeshes().back().GetVBO().ToGPU(true);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    float32 startTime = glfwGetTime();

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

    float32 lastMorphing = morphing;

    // check -10.f and 10.f :)
    ImGui::SliderFloat("Morphing coefficient", &morphing, 0.f, 1.f);

    ImGui::InputFloat3("Directional Light Direction",
                       glm::value_ptr(dirLight.GetDirection()));

    ImGui::ColorEdit3("Directional Light Ambient",
                      glm::value_ptr(dirLightColor.Ambient()));
    ImGui::ColorEdit3("Directional Light Diffuse",
                      glm::value_ptr(dirLightColor.Diffuse()));
    ImGui::ColorEdit3("Directional Light Specular",
                      glm::value_ptr(dirLightColor.Specular()));

    ImGui::ColorEdit3("Spot Light Ambient",
                      glm::value_ptr(spotLightColor.Ambient()));
    ImGui::ColorEdit3("Spot Light Diffuse",
                      glm::value_ptr(spotLightColor.Diffuse()));
    ImGui::ColorEdit3("Spot Light Specular",
                      glm::value_ptr(spotLightColor.Specular()));

    ImGui::InputFloat("Spot Light Cut Off", &cutOffDeg);
    ImGui::InputFloat("Spot Light Outer Cut Off", &outerCutOffDeg);

    ImGui::End();

    spotLight.SetCutOff(glm::cos(glm::radians(cutOffDeg)));
    spotLight.SetOuterCutOff(glm::cos(glm::radians(outerCutOffDeg)));

    InputHandler();

    dirLight.SetLight(dirLightColor);
    spotLight.SetLight(spotLightColor);
    spotLight.SetPosition(camera.GetPosition());
    spotLight.SetDirection(camera.GetDirection());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (lastMorphing != morphing) {
      Morph(from, to, modelMesh.GetIBO().GetElements(),
            modelMesh.GetVBO().GetVerticies(), morphing);
      model.GetMeshes().back().GetVBO().ToGPU(true);
    }

    shader.SetMatrix4f("camera.view", camera.GetView());
    shader.SetMatrix4f("camera.projection", camera.GetProjection());
    shader.SetFloat("material.shininess", 32);
    dirLight.Activate(shader, camera);
    spotLight.Activate(shader, camera);

    model.Draw(shader);

    app.SetDelta(glfwGetTime() - startTime);

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);

    fps++;
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

}  // namespace over

int main() {
  try {
    over::Run();
  } catch (std::exception& e) {
    fmt::println("Error: {}", e.what());
    throw;
  } catch (...) {
    fmt::println("Unexpected error...");
    throw;
  }
  return 0;
}