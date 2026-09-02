#include <exception>
#include <optional>
#include <vector>

#include <fmt/core.h>
#include <memory>

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

    auto* ctx = window.SetupGL({
        .default_buffer_allocator = std::make_unique<gl::SimpleBufferAllocator>(),
        .default_texture_2d_allocator = std::make_unique<gl::SimpleTextureAllocator>(gl::TextureType::Texture2D),
        .default_cube_map_allocator = std::make_unique<gl::SimpleTextureAllocator>(gl::TextureType::CubeMap),
        .default_texture_2d_multi_sample_allocator =
            std::make_unique<gl::SimpleTextureAllocator>(gl::TextureType::Texture2DMultiSample),
    });

    auto buf = gl::Buffer(ctx);

    std::vector<int32> vec = {0, 1, 2};
    int32 result = 0;
    gl::TypedBuffer<int32>(buf)
        .Alloc(vec.size(), vec.data(), gl::Buffer::Usage::StaticDraw)
        .Map(gl::Buffer::Access::Read,
             [&](int32* self, usize sz) {
                 for (usize i = 0; i < sz; i++) {
                     result += self[i];
                 }
             })
        .As<gl::UniformBuffer>()
        .Bind()
        .AttachUniform(0)
        .Unbind();

    fmt::println("result: {}", result);

    auto tex = gl::Texture2D(ctx);
    tex.Alloc(GL_RGB32F, {100, 100}, 6)
        .Wrap(gl::TextureWrap::Repeat, gl::TextureWrap::ToEdge)
        .Filter(gl::TextureMinFilter::Linear2, gl::TextureMagFilter::Linear)
        .Bake()
        .AttachUniform(0);

    auto cubemap = gl::CubeMap(ctx);
    cubemap.Alloc(GL_RGB32F, 1024, 5)
        .Wrap(gl::TextureWrap::ToEdge, gl::TextureWrap::ToEdge)
        .Filter(gl::TextureMinFilter::Linear, gl::TextureMagFilter::Linear)
        .Bake();

    auto multisample = gl::Texture2DMultiSample(ctx);
    multisample.Alloc(GL_RGB32F, {100, 100}, 4, false);

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
