#include <over/engine/App.hpp>
#include <over/engine/Core.hpp>

#include <memory>

#include <stb_image.h>
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
        _cubeMap(),
        _cubeMapTexturesNames({"right.jpg", "left.jpg", "top.jpg", "bottom.jpg",
                               "front.jpg", "back.jpg"}),

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

    _color.As<gl::TextureTarget::TEXTURE_2D>([&](gl::Texture2DView& self) {
      self.Reserve2D(GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
      self.GenerateMipmap();

      self.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      self.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    });

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

    _cubeMap.As<gl::TextureTarget::TEXTURE_CUBE_MAP>(
        [&](gl::CubeMapView& self) {
          for (usize i = 0; i < _cubeMapTexturesNames.size(); i++) {
            auto img = host::Image2D::FromFile("textures/skybox/" +
                                               _cubeMapTexturesNames[i]);
            auto format = gl::Texture::GetFormat(img.Channels());
            self.Reserve2DAs(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, format,
                             img.Width(), img.Height(), format,
                             GL_UNSIGNED_BYTE, img.Data().data());
          }

          self.SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          self.SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          self.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          self.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
          self.SetParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        });

    _baseShader = Shader("shaders/DimentionVertex.shader",
                         "shaders/DimentionFragment.shader");
    _screenShader =
        Shader("shaders/ScreenVertex.shader", "shaders/ScreenFragment.shader");

    _quad =
        Mesh::GenQuad({MeshTexture(_color.As<gl::TextureTarget::TEXTURE_2D>(),
                                   MeshTexture::Type::DIFFUSE)});

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

  gl::TextureWrapper<> _color;

  gl::TextureWrapper<> _cubeMap;
  std::array<std::string, 6> _cubeMapTexturesNames;

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
