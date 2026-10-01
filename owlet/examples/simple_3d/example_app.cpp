#include "example_app.h"

#include <owlet/gl/core.h>

#include "example_scene.h"

namespace example {
App::App() : engine::App<App>() {
    CreateWindow(
        {
            .width = 1980,
            .height = 980,
            .title = "Test app",
            .gl = std::make_optional(gl::Version{
                .major = 4,
                .minor = 6,
            }),
        },
        gl::Settings{
            .default_buffer_allocator = std::make_unique<gl::SimpleBufferAllocator>(),
            .default_vertex_array_allocator = std::make_unique<gl::SimpleVertexArrayAllocator>(),
            .default_vertex_shader_allocator = std::make_unique<gl::SimpleShaderAllocator>(GL_VERTEX_SHADER),
            .default_fragment_shader_allocator = std::make_unique<gl::SimpleShaderAllocator>(GL_FRAGMENT_SHADER),
            .default_program_allocator = std::make_unique<gl::SimpleProgramAllocator>(),
        });

    Scenes().Add(std::make_unique<ExampleScene>(MainWindow().GL()));
}

void App::Update(gl::Context* gl, float dt) {}
void App::OnError(int32 err_code, std::string_view description) {
    fmt::println("error: {}, {}", err_code, description);
}

void App::OnInput(os::Window& window, int32 key_code, int32 scan_code, int32 action, int32 mode) {
    fmt::println("Input on window: {} {} {} {}", key_code, scan_code, action, mode);

    if (key_code == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        MainWindow().SetShouldClose(true);
    }
}

void App::OnMouseButton(os::Window& window, int32 button_key, int32 button_action, int32 button_mods) {}
void App::OnCursorPosition(os::Window& window, float cursor_x, float cursor_y) {}
void App::OnCursorPositionChanged(os::Window& window, float delta_x, float delta_y) {}
}    // namespace example
