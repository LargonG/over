#include <owlet/os/monitor.h>

#include <GLFW/glfw3.h>

namespace owlet::os {
Monitor::Monitor() : m_self(nullptr) {}

Monitor::Monitor(GLFWmonitor* monitor) : m_self(monitor) {}

}    // namespace owlet::os
