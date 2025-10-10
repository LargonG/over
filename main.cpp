
#include "actor.hpp"
#include "fmt/base.h"

int main() {
  over::Actor actor;

  actor.Tick();

  fmt::print("Hello, world!");

  return 0;
}