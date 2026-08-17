#include <exception>
#include <optional>
#include <vector>

#include <fmt/core.h>

#include <fmt/format.h>
#include <owlet/gl/context.h>
#include <owlet/gl/core.h>
#include <owlet/os/core.h>
#include <owlet/types.h>

namespace example {

void Run() {
    using namespace owlet;

    os::Desktop desktop;

    auto monitors = desktop.AvailableMonitors();

    for (usize i = 0; i < monitors.size(); i++) {
        fmt::println("Monitor: {}", reinterpret_cast<void*>(monitors[i].Raw()));
    }

    auto err_call = [](int err, const char* desc) {
        auto msg = fmt::format("Error code: {}, description: {}", err, desc);
        throw std::runtime_error(msg);
    };
    auto key_call = [](GLFWwindow* window, int key, int scancode, int action, int mode) noexcept {
        fmt::println("Key code: {}", key);
        fmt::println("Scancode: {}", scancode);
        fmt::println("Action: {}", action);
        fmt::println("Mode: {}", mode);

        if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
            glfwSetWindowShouldClose(window, true);
        }
    };
    auto window = os::Window(
        os::Window::Settings{
            .width = 1980,
            .height = 920,
            .title = "Simple example",
            .gl = std::make_optional(gl::Settings{
                .default_buffer_allocator = new gl::SimpleBufferAllocator(),
            }),
        },
        err_call, key_call);

    auto* ctx = window.GL();

    auto buf = gl::Buffer(ctx);
    std::vector<int32> vec = {0, 1, 2, 3};
    buf.Write(vec.size() * sizeof(int32), vec.data(), gl::Buffer::Usage::StaticDraw);

    int32 result = 0;

    buf.Map<int32>(gl::Buffer::Access::Read, [&](auto* self) {
        for (usize i = 0; i < vec.size(); i++) {
            result += self[i];
        }
    });

    fmt::println("result: {}", result);

    while (!window.ShouldClose()) {
        desktop.PollEvents();

        ctx->ClearColor(0.3f, 0.0, 0.0, 1.0);
        ctx->Clear(GL_COLOR_BUFFER_BIT);

        window.SwapBuffers();
    }
}
}    // namespace example

int main() {
    try {
        example::Run();
    } catch (std::exception& e) {
        fmt::println("Error: {}", e.what());
    } catch (...) {
        fmt::println("Unknown error");
    }

    return 0;
}
