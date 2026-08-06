module;

#include <exception>
#include <source_location>
#include <stdexcept>
#include <utility>

#include <fmt/core.h>
#include <glad/gl.h>

export module owlet.debug;

namespace lowl {
constexpr bool ShouldDebug = true;
}

export namespace lowl {
#if _DEBUG || !NDEBUG
export constexpr bool Debug = ShouldDebug;
#else
export constexpr bool Debug = false;
#endif

/// <summary>
/// Assert checks that expression is true. 
/// You don't need to make functions noexcept(false) if you use Assert inside them
/// because Assert terminates (and does not throw exception) when expression is false
/// </summary>
/// <typeparam name="Debug">Debug flag, optional, configurable</typeparam>
/// <param name="expression"></param>
/// <param name="location"></param>
/// <returns></returns>
export template <bool Debug = Debug>
constexpr auto Assert(bool expression, const std::source_location location = std::source_location::current()) noexcept {
    if constexpr (Debug) {
        if (!expression) {
            fmt::println("Assertion error at position: {}:{}", location.file_name(), location.line());
            std::terminate();
        }
    }
}

export constexpr auto Assert(bool expression,
                             const std::source_location location = std::source_location::current()) noexcept {
    Assert<Debug>(expression, location);
}

}    // namespace lowl

namespace lowl::gl {
constexpr auto ErrorName(GLenum value) noexcept {
    switch (value) {
        case GL_NO_ERROR:
            return "GL_NO_ERROR";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_STACK_UNDERFLOW:
            return "GL_STACK_UNDERFLOW";
        case GL_STACK_OVERFLOW:
            return "GL_STACK_OVERFLOW";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        default:
            return "UNKNOWN_GL_ERROR";
    }
}
}    // namespace lowl::gl

export namespace lowl::gl {
export template <bool Debug = Debug>
constexpr auto AssertGL(GladGLContext* const ctx,
                        const std::source_location location = std::source_location::current()) noexcept {
    if constexpr (Debug) {
        auto err = ctx->GetError();
        if (err != GL_NO_ERROR) {
            fmt::println("OpenGL error: {} (code {}), at position: {}:{}", ErrorName(err), err, location.file_name(),
                         location.line());
            std::terminate();
        }
    }
}

constexpr auto AssertGL(GladGLContext* const ctx,
                        const std::source_location location = std::source_location::current()) noexcept {
    AssertGL<Debug>(ctx, location);
}

}    // namespace lowl::gl
