#include <owlet/debug/log.h>

#include <string_view>

#include <owlet/debug/require.h>

namespace owlet::utils {
std::string_view LevelName(debug::Level level) {
    switch (level) {
        case debug::Level::Error:
            return "ERROR";
        case debug::Level::Warning:
            return "WARNING";
        case debug::Level::Info:
            return "INFO";
    }
    debug::Unreachable("Unknown debug log level name");
}
}    // namespace owlet::utils

namespace owlet::debug {
template struct Logger<Level>;
}
