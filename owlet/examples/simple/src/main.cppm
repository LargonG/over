module;

#include <cstdio>
#include <exception>
#include <utility>

#include <fmt/core.h>

export module app;

import owlet.desktop;
import owlet.gl;

namespace example {

void Run() {
    using namespace lowl;

    desktop::Desktop os;

    desktop::WindowParameters params{
        .width = 1980, .height = 1080, .title = "Simple program", .gl_version_major = 4, .gl_version_minor = 6};

    desktop::Window window(params);

    auto ctx = gl::Context::FromWindow(window, [&](auto& context) {
        return gl::ContextParameters{.default_buffer_allocator = new gl::BufferAllocator(context)};
    });

    auto [major, minor] = ctx.Version();
    fmt::println("GL version: {} {}", major, minor);

    auto buffer = gl::Buffer(ctx);

    buffer.Data<int32>(4, nullptr, gl::Buffer::Usage::StaticDraw);

    auto value = buffer.Map<float32>(gl::Buffer::Access::All, [&](auto* arr) {
        float32 result = 0;
        for (usize i = 0; i < buffer.Size<float32>(); i++) {
            arr[i] = i * i + 0.3;
            result += arr[i];
        }
        return result;
    });

    fmt::println("{}", value);

    while (!window.ShouldClose()) {
        os.PollEvents();

        ctx.Clear({gl::BufferBit::Color});

        window.SwapBuffers();
    }
}
}    // namespace example

export int main() {
    try {
        example::Run();
    } catch (std::exception& e) {
        fmt::println("Error: {}", e.what());
    } catch (...) {
        fmt::println("Unknown error");
    }

    return 0;
}
