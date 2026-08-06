module;

#include <cassert>
#include <stdexcept>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

export module owlet.desktop;

export import :window;
export import :monitor;
import owlet.debug;

namespace lowl::desktop {
export class Desktop {
  public:
    Desktop();

    Desktop(const Desktop&) = delete;

    ~Desktop();

    auto PollEvents() -> void;
    auto AvailableMonitors() -> std::vector<Monitor>;

    auto Instance() {
        Assert<>(s_instance);
        return s_instance;
    }

  private:
    static Desktop* s_instance;
};
}    // namespace lowl::desktop

module :private;

import owlet.types;

namespace lowl::desktop {

Desktop* Desktop::s_instance = nullptr;

Desktop::Desktop() {
    if (s_instance) {
        throw std::runtime_error("GLFW context has already been initialized");
    }

    if (!glfwInit()) {
        throw std::runtime_error("Cannot initialize GLFW context");
    }
    s_instance = this;
}

Desktop::~Desktop() {
    glfwTerminate();
}

auto Desktop::PollEvents() -> void {
    glfwPollEvents();
}

auto Desktop::AvailableMonitors() -> std::vector<Monitor> {
    int32 cnt;
    auto monitors = glfwGetMonitors(&cnt);
    std::vector<Monitor> result(cnt);
    for (int32 i = 0; i < cnt; i++) {
        result[i] = MonitorFromGLFW(monitors[i]);
    }
    return result;
}

}    // namespace lowl::desktop
