#pragma once

#include <owlet/3d/core.h>
#include <owlet/controllers/fpv_controller.h>
#include <owlet/engine/scene.h>
#include <owlet/gl/core.h>

namespace example {
using namespace owlet;

struct ExampleScene : engine::Scene {
    ExampleScene(gl::Context* gl);
    // Inherited via Scene
    void Load() override;
    void Unload() override;

    void Update(float dt) override;
    void OnCursorPositionChanged(float delta_x, float delta_y);

  protected:
    d3::SpaceSettings m_settings;
    d3::Camera m_camera;
    d3::Transform m_camera_transform;
    controllers::FpvController m_controller;
};
}    // namespace example
