#include <over/engine/App.hpp>
#include <over/engine/Core.hpp>

#include <over/core/Mesh.hpp>
#include <over/core/Shader.hpp>
#include <over/core/opengl/Framebuffer.hpp>

namespace over {

class FeaturesApp : public App {
 public:
  FeaturesApp()
      : App("features"),
        _buffer(),
        _color(),
        _rbuffer(),
        _screenShader(),
        _quad() {}

  void Init() override {
    PrintName();

    _ctx.SetDepthTest(true);
    _ctx.SetFaceCulling(true);

    auto width = _window.GetWidth();
    auto height = _window.GetHeight();

    _color = Texture2D(width, height, nullptr, GL_RGB);
    _rbuffer = RenderBuffer(width, height, GL_DEPTH24_STENCIL8);

    _buffer.Setup();

    _buffer.Use([&]() {
      _buffer.Attach(GL_COLOR_ATTACHMENT0, _color);
      _buffer.Attach(GL_DEPTH_STENCIL_ATTACHMENT, _rbuffer);

      if (!_buffer.IsReady()) {
        throw std::runtime_error("Cannot create frame buffer");
      }
    });

    _screenShader =
        Shader("shaders/MainVertex.shader", "shaders/MainFragment.shader");

    _quad = Mesh::GenQuad({static_cast<Texture>(_color)});
  }

  void Update(float32 dt) override {
    _ctx.ClearAll();
    _buffer.Use([&]() {
      _ctx.SetClearColor({0.f, 0.f, 0.25f, 1.f});
      _ctx.SetDepthTest(true);
      _ctx.ClearAll();
    });

    _ctx.SetClearColor({1.f, 1.f, 1.f, 1.f});
    _ctx.SetDepthTest(false);
    _ctx.ClearAll();

    // to default screen framebuffer
    _screenShader.Use([&]() {
      _quad.Draw(_screenShader);  // looks terrible, fix?
    });

    fmt::println("fps: {}", _fps);
  }

 private:
  Shader _screenShader;
  Framebuffer _buffer;
  Texture2D _color;
  RenderBuffer _rbuffer;
  Mesh _quad;
};

void Run() {
  FeaturesApp app;
  app.Run(1980, 960);
}

}  // namespace over
