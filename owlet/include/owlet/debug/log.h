#pragma once

#include <memory>
#include <optional>
#include <ostream>
#include <string_view>

#include <owlet/collections.h>
#include <owlet/debug/core.h>

namespace owlet::debug {
enum class Level {
    Error,
    Warning,
    Info,
};
}

namespace owlet::utils {
std::string_view LevelName(debug::Level level);
}    // namespace owlet::utils

namespace owlet::debug {

template <class Level>
struct Logger {
  public:
    Logger(std::unique_ptr<std::ostream>&& output) : m_output(std::move(output)) {}

    void Log(Level level, std::string_view value) {
        if (m_enabled.contains(level)) {
            *m_output << "[" << ::owlet::utils::LevelName(level) << "] " << value << "\n";
        }
    }

    void Enable(Level level) { m_enabled.insert(level); }

  private:
    std::unique_ptr<std::ostream> m_output;
    hashset<Level> m_enabled;
};

using InternalLogger = Logger<Level>;
extern template struct Logger<Level>;

inline InternalLogger* DefaultLogger(std::optional<InternalLogger*> val = {}) {
    static InternalLogger* logger;
    if (val.has_value()) {
        logger = val.value();
    }

    return logger;
}
}    // namespace owlet::debug
