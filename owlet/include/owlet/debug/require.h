#pragma once

#include <source_location>
#include <string_view>

namespace owlet::debug {
inline void Require(bool expression, std::string_view description,
                    std::source_location source = std::source_location::current()) {
    if (!expression) {
        fmt::println("Requirement error: {}\nLine: {}:{}", description, source.file_name(), source.line());
        std::terminate();
    }
}
}    // namespace owlet::debug
