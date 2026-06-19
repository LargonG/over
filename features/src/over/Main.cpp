#include <over/engine/App.hpp>
#include <over/engine/Core.hpp>

#include <memory>

#include <over/core/Camera.hpp>
#include <over/core/Mesh.hpp>
#include <over/core/Model.hpp>
#include <over/core/Shader.hpp>
#include <over/core/opengl/Framebuffer.hpp>

namespace over {

class FeaturesApp : public App {
 public:
  FeaturesApp()
      : App("features"),
        _baseShader(),
        _screenShader(),

        _buffer(),
        _defaultBuffer(),

        _color(),
        _rbuffer(),
        _quad(),
        _model(nullptr),

        _camera({0.f, 0.f, 3.f}, {0.f, 0.f, 0.f}, 25.f, 120.f, 1.f),

        _elapsedTime(0.f) {}

  void Init() override {
    PrintName();

    _input.SetCursor(false);

    auto width = _window.GetWidth();
    auto height = _window.GetHeight();

    _color = Texture2D(width, height, nullptr, GL_RGB);
    _rbuffer = RenderBuffer(width, height, GL_DEPTH24_STENCIL8);

    _camera.SetAspectRatio(width * 1.f / height);

    _buffer.Setup();

    _buffer.Use([&]() {
      _buffer.Attach(GL_COLOR_ATTACHMENT0, _color);
      _buffer.Attach(GL_DEPTH_STENCIL_ATTACHMENT, _rbuffer);

      if (!_buffer.IsReady()) {
        throw std::runtime_error("Cannot create frame buffer");
      }
    });

    _baseShader = Shader("shaders/DimentionVertex.shader",
                         "shaders/DimentionFragment.shader");
    _screenShader =
        Shader("shaders/ScreenVertex.shader", "shaders/ScreenFragment.shader");

    _quad = Mesh::GenQuad({static_cast<Texture>(_color)});

    _model = std::make_unique<Model>(
        "resources/backpack/backpack.obj");  // std::make_unique<Model>("resources/cube/cube.glb");
  }

  void Update(float32 dt) override {
    if (Input::Instance().IsPressed(Input::Key::ESCAPE)) {
      _window.SetShouldClose(true);
    }

    _camera.SetSpeed(25.f);
    if (Input::Instance().IsPressed(Input::Key::LEFT_CONTROL)) {
      _camera.SetSpeed(5.f);
    }

    _camera.UpdatePositionCallback(_window.Get(), dt);
    auto [xpos, ypos] = Input::Instance().GetCursorPosition();
    _camera.UpdateYawPitchCallback(xpos, ypos);

    _buffer.Use([&]() {
      _baseShader.Use([&] {
        _ctx.SetClearColor({0.f, 0.5f, 0.5f, 1.f});
        _ctx.SetFaceCulling(true);
        _ctx.SetDepthTest(true);
        _ctx.ClearAll();

        _baseShader.SetMatrix4f(
            "mvp", _camera.GetProjection() *
                       (_camera.GetView() * _model->GetTransform().GetModel()));

        _model->Draw();
      });
    });

    // to default screen framebuffer
    _defaultBuffer.Use([&]() {
      _screenShader.Use([&]() {
        _ctx.SetClearColor({1.f, 1.f, 1.f, 1.f});
        _ctx.SetDepthTest(false);
        _ctx.ClearAll();

        _inverted = false;
        if (Input::Instance().IsPressed(Input::Key::LEFT_SHIFT)) {
          _inverted = true;
        }
        _screenShader.SetBool("material.inverted", _inverted);

        _quad.Draw();
      });
    });

    _elapsedTime += dt;
    if (_elapsedTime >= 1.f) {
      _elapsedTime = 0.f;
      fmt::println("fps: {}", _fps);
    }
  }

 private:
  Shader _baseShader;
  Shader _screenShader;

  Framebuffer _buffer;
  Framebuffer _defaultBuffer;

  Texture2D _color;
  RenderBuffer _rbuffer;
  Mesh _quad;
  std::unique_ptr<Model> _model;

  Camera _camera;

  float32 _elapsedTime;

  bool _inverted = false;
};

void Run() {
  FeaturesApp app;
  app.Run(1980, 960);
}

}  // namespace over
