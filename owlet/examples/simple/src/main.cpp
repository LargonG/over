#include <exception>
#include <optional>
#include <vector>

#include <fmt/core.h>

#include <fmt/format.h>
#include <owlet/gl/core.h>
#include <owlet/gl/targets/core.h>
#include <owlet/gl/utils/typed_buffer.h>
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
            .gl = std::make_optional<gl::Version>({4, 6}),
        },
        err_call, key_call);

    auto* ctx = window.GL({.default_buffer_allocator = std::make_unique_for_overwrite<gl::SimpleBufferAllocator>()});

    auto buf = gl::Buffer(ctx);

    std::vector<int32> vec = {0, 1, 2};
    buf.Alloc(vec.size() * sizeof(int32), vec.data(), gl::Buffer::Usage::StaticDraw);

    int32 result = 0;

    buf.As<gl::IndexBuffer>([&](gl::IndexBuffer idxbuf) { idxbuf.Do([&] {}); });

    auto typed = gl::TypedBuffer<int32>(buf);

    typed.Map(gl::Buffer::Access::Read, [&](int32* self, usize sz) {
        for (usize i = 0; i < sz; i++) {
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
