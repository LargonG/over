#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string_view>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

#include <owlet/gl/buffer.h>
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
    };

    Window(const Settings settings, GLFWerrorfun error_callback, GLFWkeyfun input_callback);

    Window(const Window&) = delete;

    ~Window();

    [[nodiscard]] auto Raw() const noexcept { return m_self; }

    void SwapBuffers();

    void SetShouldClose(bool value);
    [[nodiscard]] bool ShouldClose() const noexcept;

    void SetCurrent();

    gl::Context* GL(gl::Settings&& settings);

  private:
    void Free() noexcept;

    void CreateContext(gl::Settings&&);

    std::optional<gl::Version> m_gl_version;

    GLFWwindow* m_self;
    std::unique_ptr<gl::Context> m_gl;
};
}    // namespace owlet::os
