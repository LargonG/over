#pragma once

#include <memory>
#include <vector>

#include "system.hpp"

namespace over {

class Actor {
 public:
  Actor();

  Actor(const Actor&) = delete;
  Actor(Actor&&) = delete;

  Actor& operator=(const Actor&) = delete;
  Actor& operator=(Actor&&) = delete;

  void Tick();

 private:
  std::vector<std::unique_ptr<System>> systems_;
};
}  // namespace over