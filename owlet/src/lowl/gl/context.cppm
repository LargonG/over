module;

#include <initializer_list>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

export module owlet.gl:context;

import owlet.types;
import owlet.desktop;
import owlet.debug;
import :parameters;

namespace lowl::gl {

#pragma region GladGLContext ctors
// Cannot be made static!
auto DeleteContext(GladGLContext* gl) noexcept {
    operator delete(reinterpret_cast<void*>(gl));
}

auto NewContext() {
    GladGLContext* gl = reinterpret_cast<GladGLContext*>(operator new(sizeof(GladGLContext)));
    int32 version = gladLoadGLContext(gl, glfwGetProcAddress);
    if (version == 0) {
        DeleteContext(gl);
        throw std::runtime_error("Cannot load OpenGL");
    }
    return std::make_tuple(gl, version);
}
#pragma endregion

}    // namespace lowl::gl

namespace lowl::gl {

export enum class BufferBit {
    Color = GL_COLOR_BUFFER_BIT,
    Depth = GL_DEPTH_BUFFER_BIT,
    Stencil = GL_STENCIL_BUFFER_BIT
};

export class BufferAllocator;

/// <summary>
///  OpenGL Global State
/// </summary>
export class [[nodiscard]] Context final {
  private:
    struct GLVersion {
        int32 major;
        int32 minor;
    };

  public:
    /// <summary>
    /// Creates OpenGL context from GLFW window
    /// </summary>
    /// <param name="window">Initialized GLFW window</param>
    /// <returns>OpenGL context</returns>
    template <typename F>
        requires std::is_same_v<std::invoke_result_t<F&&, Context&>, ContextParameters>
    static auto FromWindow(desktop::Window& window, F&& config) -> Context {
        glfwMakeContextCurrent(window.Get());

        auto [gl, version] = NewContext();

        auto ctx = Context(gl, &window, {GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version)});

        auto params = std::forward<F>(config)(ctx);

        ctx.m_default_buffer_allocator = params.default_buffer_allocator;

        return ctx;
    }

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

    Context(Context&&) noexcept;
    Context& operator=(Context&&) noexcept;

    ~Context();

    auto Enable(GLenum name) -> void;
    auto Disable(GLenum name) -> void;

    auto Clear(std::initializer_list<BufferBit>) -> void;
    auto Clear(GLbitfield mask) -> void;

    [[nodiscard]] auto Version() const noexcept -> std::tuple<int32, int32> {
        return {m_version.major, m_version.minor};
    }

    /// <summary>
    /// For internal usage.
    /// Use this when you are sure what are you doing
    /// </summary>
    /// <returns>Raw OpenGL context</returns>
    [[nodiscard]] auto Get() const noexcept { return m_gl; }

    [[nodiscard]] auto DefaultBufferAllocator() const noexcept -> BufferAllocator& {
        lowl::Assert<>(m_default_buffer_allocator != nullptr);
        return *m_default_buffer_allocator;
    }

  private:
    Context(GladGLContext* context, desktop::Window* window, GLVersion version) noexcept;

    auto Assert() const -> void;

    GLVersion m_version;
    desktop::Window* m_window;
    GladGLContext* m_gl;

    BufferAllocator* m_default_buffer_allocator;
};
}    // namespace lowl::gl

namespace lowl::gl {
#pragma region Context ctors
Context::Context(GladGLContext* gl, desktop::Window* window, GLVersion version) noexcept
    : m_gl(gl), m_window(window), m_version(version), m_default_buffer_allocator(nullptr) {}

Context::Context(Context&& other) noexcept : Context(nullptr, nullptr, {0, 0}) {
    *this = std::move(other);
}

Context& Context::operator=(Context&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    // Has some state... it's bad? Maybe

    DeleteContext(m_gl);

    std::swap(m_gl, other.m_gl);
    std::swap(m_window, other.m_window);
    std::swap(m_version, other.m_version);
    std::swap(m_default_buffer_allocator, other.m_default_buffer_allocator);

    return *this;
}

Context::~Context() {
    DeleteContext(m_gl);
}
#pragma endregion

auto Context::Enable(GLenum name) -> void {
    Assert();
    m_gl->Enable(name);
    AssertGL<>(m_gl);
}

auto Context::Disable(GLenum name) -> void {
    Assert();
    m_gl->Disable(name);
    AssertGL<>(m_gl);
}

auto Context::Clear(std::initializer_list<BufferBit> bits) -> void {
    GLbitfield mask = 0;
    for (const auto& bit : bits) {
        mask |= static_cast<GLbitfield>(bit);
    }

    Clear(mask);
}

auto Context::Clear(GLbitfield mask) -> void {
    Assert();
    m_gl->Clear(mask);
    AssertGL<>(m_gl);
}

auto Context::Assert() const -> void {
    lowl::Assert<>(glfwGetCurrentContext() == m_window->Get());
}

}    // namespace lowl::gl
