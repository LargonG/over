module;
#include <memory>

#include <over/engine/Core.hpp>

export module atmosphere;

import app;

namespace over {
void Run() {
  auto app = AtmosphereApp();
  app.Run(1920, 1080);
}
}  // namespace over
