#include "actor.hpp"

#include <vector>

#include "debug_system.hpp"

namespace over {

Actor::Actor() : systems_() { systems_.push_back(new DebugSystem()); }

void Actor::Tick() {
  for (auto& system : this->systems_) {
    system->Tick();
  }
}

}  // namespace over