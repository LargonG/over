#include <owlet/os/window.h>

#include <memory>
#include <stdexcept>

#include <GLFW/glfw3.h>

#include <owlet/gl/buffer.h>
#include <owlet/gl/context.h>
#include <owlet/os/monitor.h>

namespace owlet::os {

Window::Window(const Settings settings, GLFWerrorfun error_callback, GLFWkeyfun key_callback) {
    debug::Require(settings.gl.has_value(), "Does not support other render api's for now");
    debug::Require(settings.gl.value().version.major == 4 && settings.gl.value().version.minor == 6,
                   "Support OpenGL 4.6 and above");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.gl.value_or({0, 0}).version.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.gl.value_or({0, 0}).version.minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, settings.samples);
    glfwSwapInterval(settings.swap_interval);

    glfwSetErrorCallback(error_callback);

    m_self = glfwCreateWindow(settings.width, settings.height, settings.title.data(),
                              settings.monitor.value_or(Monitor()).Raw(), nullptr);
    if (!m_self) {
        throw std::runtime_error("Cannot create window");
    }

    glfwSetKeyCallback(m_self, key_callback);

    if (settings.gl) {
        m_gl_settings = settings.gl.value();
        GL();
    }
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

gl::Context* Window::GL(std::optional<gl::Settings> settings) {
    if (!m_gl) {
        CreateContext(settings.value_or(m_gl_settings));
    }

    return m_gl.get();
}

void Window::CreateContext(gl::Settings settings) {
    SetCurrent();
    m_gl.reset();

    m_gl = std::make_unique<gl::Context>();
    auto version = gladLoadGLContext(m_gl.get(), glfwGetProcAddress);

    auto actual_version_major = GLAD_VERSION_MAJOR(version);
    auto actual_version_minor = GLAD_VERSION_MINOR(version);

    debug::Require(settings.version.major < actual_version_major ||
                       settings.version.major == actual_version_major && settings.version.minor <= actual_version_minor,
                   "Owlet support OpenGL 4.6 or greater");

    m_gl->m_default_buffer_allocator = std::unique_ptr<gl::BufferAllocator>(settings.default_buffer_allocator);
}

}    // namespace owlet::os
