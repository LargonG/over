#include "debug_system.hpp"

#include <fmt/core.h>

namespace over {
void over::DebugSystem::Tick() { fmt::println("Tick started"); }
}  // namespace over