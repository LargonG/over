#include <owlet/os/window.h>

#include <memory>
#include <stdexcept>
#include <utility>

#include <GLFW/glfw3.h>

#include <owlet/debug/core.h>
#include <owlet/gl/core.h>
#include <owlet/os/monitor.h>

namespace owlet::os {

Window::Window(const Settings settings) : m_gl_version(settings.gl), m_self(nullptr), m_gl() {
    debug::Require(settings.gl.has_value(), "Does not support other render api's for now");
    debug::Require(settings.gl.value().major == 4 && settings.gl.value().minor == 6, "Support OpenGL 4.6 and above");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.gl.value_or({0, 0}).major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.gl.value_or({0, 0}).minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, settings.samples);
    glfwSwapInterval(settings.swap_interval);

    glfwSetErrorCallback(settings.error_callback);

    m_self = glfwCreateWindow(settings.width, settings.height, settings.title.data(),
                              settings.monitor.value_or(Monitor()).Raw(), nullptr);
    debug::Require(m_self, "Cannot create window");

    glfwSetKeyCallback(m_self, settings.key_input_callback);
    glfwSetWindowSizeCallback(m_self, settings.window_resize_callback);
    glfwSetCursorPosCallback(m_self, settings.cursor_pos_callback);
}

Window::Window() noexcept : m_gl_version(), m_self(nullptr), m_gl() {}

Window::Window(Window&& other) noexcept : Window() {
    *this = std::move(other);
}

Window& Window::operator=(Window&& other) noexcept {
    m_gl_version = std::move(other.m_gl_version);
    std::swap(m_self, other.m_self);
    m_gl = std::move(other.m_gl);

    return *this;
}

Window::~Window() {
    Free();
}

void Window::Free() noexcept {
    if (!m_self) {
        return;
    }

    glfwDestroyWindow(m_self);
    m_self = nullptr;
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_self);
}

void Window::SetShouldClose(bool value) {
    glfwSetWindowShouldClose(m_self, value);
}

bool Window::ShouldClose() const noexcept {
    return glfwWindowShouldClose(m_self);
}

void Window::SetCurrent() {
    glfwMakeContextCurrent(m_self);
}

gl::Context* Window::SetupGL(gl::Settings&& settings) {
    CreateContext(std::move(settings));
    return m_gl.get();
}

std::tuple<int32, int32> Window::Size() {
    int width, height;
    glfwGetWindowSize(m_self, &width, &height);
    return {static_cast<int32>(width), static_cast<int32>(height)};
}

void Window::Resize(int32 width, int32 height) {
    glfwSetWindowSize(m_self, static_cast<int>(width), static_cast<int>(height));
}

void Window::Reposition(int32 x, int32 y) {
    glfwSetWindowPos(m_self, static_cast<int>(x), static_cast<int>(y));
}

void Window::CreateContext(gl::Settings&& settings) {
    SetCurrent();
    m_gl.reset();

    m_gl = std::make_unique<gl::Context>();
    auto version = gladLoadGLContext(m_gl.get(), glfwGetProcAddress);

    auto actual_version_major = GLAD_VERSION_MAJOR(version);
    auto actual_version_minor = GLAD_VERSION_MINOR(version);

    debug::Require(
        m_gl_version.value().major < actual_version_major ||
            m_gl_version.value().major == actual_version_major && m_gl_version.value().minor <= actual_version_minor,
        "Owlet support OpenGL 4.6 or greater");

    m_gl->m_allocators = std::move(settings);
}

}    // namespace owlet::os
