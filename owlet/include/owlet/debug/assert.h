#pragma once

#include <source_location>

#include <fmt/core.h>

#include <owlet/debug/consts.h>

namespace owlet::debug {

template <bool Debug = g_owlet_debug>
void Assert(bool expression, std::string_view description = "",
            const std::source_location& location = std::source_location::current()) noexcept {
    if constexpr (Debug) {
        if (!expression) {
            fmt::println("Assertion error: {}:{}\nDescription: {}", location.file_name(), location.line(), description);
            std::terminate();
        }
    }
}

}    // namespace owlet::debug
