#include "example_scene.h"

#include "example_app.h"

namespace example {
ExampleScene::ExampleScene(gl::Context* gl)
    : engine::Scene(gl),
      m_settings({
          .global_up = glm::vec3(0.0f, 1.0f, 0.0f),
          .global_right = glm::vec3(1.0f, 0.0f, 0.0f),
      }),
      m_camera(16.f / 9.f, glm::vec3(0), glm::vec3(0), 45.f, 0.01f, 100.0f, m_settings),
      m_camera_transform(),
      m_controller(10.f, 5.f, 2.f, 1.f, &m_camera_transform, &m_camera) {}

// Inherited via Scene
void ExampleScene::Load() {
    fmt::println("loaded");
}

void ExampleScene::Unload() {}

void ExampleScene::Update(float dt) {}

void ExampleScene::OnCursorPositionChanged(float delta_x, float delta_y) {
    auto [width, height] = example::App::Instance()->MainWindow().Size();

    float hx = delta_x / width;
    float hy = -delta_y / height;

    m_controller.Move(glm::vec2(0), glm::vec2(hx, hy));

    fmt::println("delta: {} {}", hx, hy);
}

}    // namespace example
