#pragma once

#include <vector>

#include <owlet/debug/assert.h>

namespace owlet::os {

struct Monitor;

struct Desktop {
  public:
    Desktop();

    Desktop(const Desktop&) = delete;

    ~Desktop();

    void PollEvents();
    [[nodiscard]] std::vector<Monitor> AvailableMonitors();

    [[nodiscard]] auto Instance() noexcept {
        debug::Assert(s_instance);
        return s_instance;
    }

  private:
    static Desktop* s_instance;
};
}    // namespace owlet::os
