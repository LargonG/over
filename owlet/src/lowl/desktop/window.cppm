module;

#include <optional>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <utility>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

export module owlet.desktop:window;
import owlet.types;
import :monitor;

namespace lowl::desktop {
export struct WindowParameters {
    int32 width;
    int32 height;

    std::string_view title;

    // Owlet does not support gl version less than 4.6 (maybe 4.x)
    // So, this params are prohibited to change, but exists for future maintenance
    // DSA is preferred
    int32 gl_version_major = 4;
    int32 gl_version_minor = 6;

    int32 swap_interval = 0;

    std::optional<int32> samples = std::make_optional<int32>();
    std::optional<Monitor> monitor = std::make_optional<Monitor>();

    bool s_rgb = false;
};

export class Window {
  public:
    Window(WindowParameters);
    Window() noexcept : m_ptr(nullptr) {};

    ~Window();

    Window(const Window&) = delete;
    auto operator=(const Window&) -> Window& = delete;

    Window(Window&&) noexcept;
    auto operator=(Window&&) noexcept -> Window&;

    auto ShouldClose() const -> bool;
    auto SetClose(bool value);

    auto SwapBuffers();

    auto Size() const -> std::tuple<int32, int32>;

    auto Get() const noexcept { return m_ptr; }

  private:
    auto Close() noexcept -> void;

    GLFWwindow* m_ptr;

    friend class OsContext;
};

}    // namespace lowl::desktop

// implementation
namespace lowl::desktop {
#pragma region ctors
Window::Window(WindowParameters params) : m_ptr(nullptr) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, params.gl_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, params.gl_version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, params.samples.value_or(0));
    glfwWindowHint(GLFW_SRGB_CAPABLE, params.s_rgb ? GLFW_TRUE : GLFW_FALSE);

    glfwSwapInterval(params.swap_interval);

    auto monitor = params.monitor.has_value() ? params.monitor.value().Get() : nullptr;

    m_ptr = glfwCreateWindow(params.width, params.height, params.title.data(), monitor, nullptr);
    if (!m_ptr) {
        throw std::runtime_error("Cannot create GLFW Window");
    }
}

Window::Window(Window&& other) noexcept : Window() {
    *this = std::move(other);
}

auto Window::operator=(Window&& other) noexcept -> Window& {
    if (this == &other) {
        return *this;
    }

    if (m_ptr != other.m_ptr) {
        Close();
    }

    m_ptr = std::exchange(other.m_ptr, nullptr);

    return *this;
}

Window::~Window() {
    Close();
}
#pragma endregion

auto Window::ShouldClose() const -> bool {
    return static_cast<bool>(glfwWindowShouldClose(m_ptr));
}

auto Window::SetClose(bool value) {
    glfwSetWindowShouldClose(m_ptr, value);
}

auto Window::Close() noexcept -> void {
    if (!m_ptr) {
        return;
    }

    glfwDestroyWindow(m_ptr);
}

auto Window::SwapBuffers() {
    glfwSwapBuffers(m_ptr);
}

auto Window::Size() const -> std::tuple<int32, int32> {
    int32 width, height;
    width = height = 0;

    glfwGetFramebufferSize(m_ptr, &width, &height);

    return std::make_tuple(width, height);
}
}    // namespace lowl::desktop
