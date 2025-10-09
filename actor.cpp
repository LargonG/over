#include "actor.hpp"

#include <memory>

#include "debug_system.hpp"
#include "system.hpp"

namespace over {

Actor::Actor() : systems_() {
  systems_.push_back(std::make_unique<DebugSystem>());
}

void Actor::Tick() {
  for (auto& system : this->systems_) {
    system->Tick();
  }
}

}  // namespace over