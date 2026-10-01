#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string_view>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include <owlet/debug/core.h>
#include <owlet/gl/context.h>
#include <owlet/os/monitor.h>
#include <owlet/types.h>

namespace owlet::os {

struct Monitor;

struct [[nodiscard]] Window {
  public:
    struct Settings {
        int32 width;
        int32 height;
        std::string_view title;

        int32 samples = 0;
        int32 swap_interval = 1;

        std::optional<gl::Version> gl = {};

        std::optional<Monitor> monitor = {};

        GLFWerrorfun error_callback = nullptr;
        GLFWkeyfun key_input_callback = nullptr;
        GLFWmousebuttonfun mouse_button_callback = nullptr;
        GLFWcursorposfun cursor_pos_callback = nullptr;
        GLFWwindowsizefun window_resize_callback = nullptr;
    };

    explicit Window(const Settings settings);

    Window(const Window&) = delete;

    Window(Window&&) noexcept;
    Window& operator=(Window&&) noexcept;

    ~Window();

    [[nodiscard]] auto Raw() const noexcept { return m_self; }

    void SwapBuffers();

    void SetShouldClose(bool value);
    [[nodiscard]] bool ShouldClose() const noexcept;

    void SetCurrent();

    gl::Context* SetupGL(gl::Settings&& settings);
    auto* GL() const noexcept {
        debug::Require(m_gl.get() != nullptr, "GL context has to be initialized");
        return m_gl.get();
    }

    std::tuple<int32, int32> Size();
    void Resize(int32 width, int32 height);
    void Reposition(int32 x, int32 y);

  private:
    Window() noexcept;

    void Free() noexcept;

    void CreateContext(gl::Settings&&);

    std::optional<gl::Version> m_gl_version;

    GLFWwindow* m_self;
    std::unique_ptr<gl::Context> m_gl;
};
}    // namespace owlet::os
