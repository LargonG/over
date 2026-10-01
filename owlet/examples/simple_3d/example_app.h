#pragma once

#include <owlet/engine/app.h>

namespace example {
using namespace owlet;

struct App : engine::App<App> {
    App();

    void Update(gl::Context* gl, float dt);
    void OnError(int32 err_code, std::string_view description);

    void OnInput(os::Window& window, int32 key_code, int32 scan_code, int32 action, int32 mode);

    void OnMouseButton(os::Window& window, int32 button_key, int32 button_action, int32 button_mods);
    void OnCursorPosition(os::Window& window, float cursor_x, float cursor_y);
    void OnCursorPositionChanged(os::Window& window, float delta_x, float delta_y);
};
}    // namespace example
