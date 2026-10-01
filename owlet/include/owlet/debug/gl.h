#pragma once

#include <source_location>
#include <string_view>
#include <utility>

#include <fmt/core.h>

#include <glad/gl.h>
#include <owlet/debug/consts.h>
#include <owlet/gl/context.h>

namespace owlet::debug {

std::string_view ErrorName(GLenum err) noexcept;

template <bool Debug = g_owlet_debug>
void GLCheckError(gl::Context* context, std::string_view description = "not provided",
                  std::source_location src = std::source_location::current()) noexcept {
    if constexpr (Debug) {
        auto err = context->GetError();
        if (err != GL_NO_ERROR) {
            fmt::println("GL Error: {} (code {}) at: {}:{};\nDescription: {}", ErrorName(err), err, src.file_name(),
                         src.line(), description);
            std::terminate();
        }
    }
}

}    // namespace owlet::debug
