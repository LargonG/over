#include <exception>
#include <fstream>
#include <memory>
#include <optional>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>

#include <owlet/debug/log.h>
#include <owlet/gl/core.h>
#include <owlet/gl/state.h>
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
        using namespace debug;

        auto& log = *debug::DefaultLogger();

        log.Log(Level::Info,
                fmt::format("Key code: {}\nScan code: {}\nAction: {}\nMode: {}", key, scancode, action, mode));

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

    auto* ctx = window.SetupGL(
        {.default_buffer_allocator = std::make_unique<gl::SimpleBufferAllocator>(),
         .default_texture_2d_allocator = std::make_unique<gl::SimpleTextureAllocator>(gl::TextureType::Texture2D),
         .default_cube_map_allocator = std::make_unique<gl::SimpleTextureAllocator>(gl::TextureType::CubeMap),
         .default_texture_2d_multi_sample_allocator =
             std::make_unique<gl::SimpleTextureAllocator>(gl::TextureType::Texture2DMultiSample),
         .default_vertex_array_allocator = std::make_unique<gl::SimpleVertexArrayAllocator>()});

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

    auto logger = std::make_unique<debug::InternalLogger>(std::make_unique<std::ofstream>("log.txt"));

    auto main_state = gl::State(ctx);
    main_state.Enable({GL_DEPTH_TEST, GL_CULL_FACE, GL_BLEND});

    auto obj = gl::VertexArray(ctx);
    obj.Attach(0, buf, 0, sizeof(float32) * 3);
    obj.Format(0, GL_FLOAT, 3, 0);
    obj.BindFormats(0, {0});

    auto main_layout = gl::Layout();
    main_layout.Set(obj);

    auto& log = *debug::DefaultLogger(logger.get());

    log.Enable(debug::Level::Info);
    log.Log(debug::Level::Info, "Hello");

    while (!window.ShouldClose()) {
        desktop.PollEvents();

        ctx->ClearColor(0.3f, 0.0, 0.0, 1.0);
        ctx->Clear(GL_COLOR_BUFFER_BIT);

        main_state.Begin(nullptr);

        main_layout.Begin(nullptr);

        main_layout.End();

        main_state.End();

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
