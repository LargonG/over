module;

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

export module owlet.desktop:monitor;
import owlet.types;

namespace lowl::desktop {
export class Monitor {
  public:
    Monitor() noexcept : m_monitor(nullptr) {}

    Monitor(const Monitor&) = default;
    Monitor& operator=(const Monitor&) = default;

    Monitor(Monitor&&) noexcept = default;
    Monitor& operator=(Monitor&&) noexcept = default;

    auto Get() const noexcept { return m_monitor; }

  private:
    GLFWmonitor* m_monitor;

    explicit Monitor(GLFWmonitor*) noexcept;

    friend auto MonitorFromGLFW(GLFWmonitor*) noexcept -> Monitor;
};
}    // namespace lowl::desktop

namespace lowl::desktop {

Monitor::Monitor(GLFWmonitor* monitor) noexcept : m_monitor(monitor) {}

// Declaration/Implementation is available only for Desktop
auto MonitorFromGLFW(GLFWmonitor* monitor) noexcept -> Monitor {
    return Monitor(monitor);
}
}    // namespace lowl::desktop
