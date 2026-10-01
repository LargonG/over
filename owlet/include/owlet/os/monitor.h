#pragma once

#include <owlet/types.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

namespace owlet::os {

struct Monitor {
  public:
    Monitor();

    Monitor(const Monitor&) = default;
    Monitor(Monitor&&) = default;

    auto Raw() { return m_self; }

  private:
    Monitor(GLFWmonitor* monitor);

    GLFWmonitor* m_self;

    friend struct Desktop;
};

}    // namespace owlet::os
