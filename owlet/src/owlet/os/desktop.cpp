#include <owlet/os/desktop.h>

#include <vector>

#include <owlet/debug/core.h>
#include <owlet/os/monitor.h>
#include <owlet/types.h>

#include <GLFW/glfw3.h>

namespace owlet::os {
Desktop* Desktop::s_instance = nullptr;

Desktop::Desktop() {
    debug::Assert(!s_instance);
    glfwInit();
    s_instance = this;
}

Desktop::~Desktop() {
    debug::Assert(s_instance == this);
    s_instance = nullptr;
    glfwTerminate();
}

auto Desktop::PollEvents() -> void {
    glfwPollEvents();
}

auto Desktop::AvailableMonitors() -> std::vector<Monitor> {
    int32 size;
    auto** monitors = glfwGetMonitors(&size);
    debug::Assert(monitors);

    std::vector<Monitor> result;
    result.reserve(size);

    for (int32 i = 0; i < size; i++) {
        auto monitor = Monitor(monitors[i]);
        result.push_back(monitor);
    }

    return result;
}
}    // namespace owlet::os
