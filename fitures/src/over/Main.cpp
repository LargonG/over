#include <over/engine/App.hpp>
#include <over/engine/Core.hpp>

namespace over {

class FituresApp : public App {
 public:
  FituresApp() : App("fitures") {}

  void Init() override { PrintName(); }

  void Update(float32 dt) override { fmt::println("fps: {}", _fps); }
};

void Run() {
  FituresApp app;
  app.Run(1980, 960);
}
}  // namespace over
