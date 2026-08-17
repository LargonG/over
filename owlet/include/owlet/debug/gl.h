#pragma once

#include <glad/gl.h>

#include <owlet/debug/core.h>
#include <owlet/gl/context.h>

namespace owlet::debug {

template <bool Debug = g_owlet_debug>
void GLCheckError(gl::Context* context) {
    if constexpr (Debug) {
        GLint err = context->GetError();
        if (err != GL_NO_ERROR) {
            fmt::println("Error: {} (code {})", "", err);
            std::terminate();
        }
    }
}
}    // namespace owlet::debug
