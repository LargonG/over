#pragma once

#include <owlet/gl/context.h>
#include <owlet/types.h>

namespace owlet::engine {
struct Scene {
  public:
    Scene(gl::Context* gl);

    Scene(const Scene&) = delete;

    Scene(Scene&&) noexcept = default;

    virtual void Load() = 0;
    virtual void Unload() = 0;

    virtual void Update([[maybe_unused]] float dt) {}
    virtual void OnInput([[maybe_unused]] int32 key_code, [[maybe_unused]] int32 scan_code,
                         [[maybe_unused]] int32 action, [[maybe_unused]] int32 mode) {}
    virtual void OnMouseButton([[maybe_unused]] int32 button_code, [[maybe_unused]] int32 button_action,
                               [[maybe_unused]] int32 button_mode) {}
    virtual void OnCursorPosition([[maybe_unused]] float pos_x, [[maybe_unused]] float pos_y) {}
    virtual void OnCursorPositionChanged([[maybe_unused]] float delta_x, [[maybe_unused]] float delta_y) {}

    virtual ~Scene() {}

  private:
    gl::Context* m_gl;
};
}    // namespace owlet::engine
