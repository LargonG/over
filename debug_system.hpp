#pragma once
#include "system.hpp"

namespace over {
class DebugSystem : public System {
 public:
  void Tick() override;

 private:
};
}  // namespace over