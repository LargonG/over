#include <over/engine/App.hpp>
#include <over/engine/Core.hpp>

#include <memory>

#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>
#include <over/core/Camera.hpp>
#include <over/core/Mesh.hpp>
#include <over/core/Model.hpp>
#include <over/core/Shader.hpp>
#include <over/core/opengl/Framebuffer.hpp>
#include <over/core/opengl/views/FrameBufferView.hpp>
#include <over/core/opengl/views/RenderBufferView.hpp>

namespace over {

class FeaturesApp : public App {
 public:
  FeaturesApp()
      : App("features"),
        _baseShader(),
        _screenShader(),
        _skyboxShader(),

        _frame(),

        _color(),
        _cubeMap(),
        _cubeMapTexturesNames({"right.jpg", "left.jpg", "top.jpg", "bottom.jpg",
                               "front.jpg", "back.jpg"}),

        _rbuffer(),
        _quad(),
        _model(nullptr),

        _camera({0.f, 0.f, 3.f}, {0.f, 0.f, 0.f}, 25.f, 45.f, 16.f / 9.f),
        _cameraBuffer(),

        //_camera({0.f, 0.f, 3.f}, {0.f, 0.f, 0.f}, 25.f, 90.f, 1.f),

        _elapsedTime(0.f),
        _explode(0.f),

        _debug(false) {}

  void Init() override {
    PrintName();
    _input.SetCursor(false);

    auto width = _window.GetWidth();
    auto height = _window.GetHeight();

    auto colorTexture2D = _color.As<gl::AsTexture2D>();
    auto renderBuffer = _rbuffer.As<gl::RenderBufferTarget::RENDER_BUFFER>();

    colorTexture2D.Use([&](gl::Texture2DView& self) {
      self.Reserve2D(GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
      self.GenerateMipmap();

      self.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      self.SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      self.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      self.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    });

    renderBuffer.Use(
        [&](gl::RenderBufferView<gl::RenderBufferTarget::RENDER_BUFFER>& self) {
          self.Reserve(GL_DEPTH24_STENCIL8, width, height);
        });

    _camera.SetAspectRatio(width * 1.f / height);

    _frame.As<gl::FrameBufferTarget::FRAMEBUFFER>(
        [&](gl::FrameBufferView<gl::FrameBufferTarget::FRAMEBUFFER>& self) {
          self.Attach(GL_COLOR_ATTACHMENT0, colorTexture2D, 0);
          self.Attach(GL_DEPTH_STENCIL_ATTACHMENT, renderBuffer);

          self.ReadyOrThrow("Cannot create frame buffer");
        });

    _cubeMap.As<gl::TextureTarget::TEXTURE_CUBE_MAP>(
        [&](gl::CubeMapView& self) {
          for (usize i = 0; i < _cubeMapTexturesNames.size(); i++) {
            auto img = host::Image2D::FromFile("resources/textures/skybox/" +
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

    _baseShader = Shader("shaders/DimensionVertex.shader",
                         "shaders/DimensionFragment.shader",
                         "shaders/DimensionGeometry.shader");
    _screenShader =
        Shader("shaders/ScreenVertex.shader", "shaders/ScreenFragment.shader");

    _quad =
        Mesh::GenQuad({MeshTexture(_color.As<gl::TextureTarget::TEXTURE_2D>(),
                                   MeshTexture::Type::DIFFUSE)});

    _model = std::make_unique<Model>(
        "resources/backpack/backpack.obj");  // std::make_unique<Model>("resources/cube/cube.glb");

    // TODO: make some "shape" class
    constexpr std::array<float32,
                         /* rows */ 6 * /* blocks*/ 6 * /* per vertex */ 3>
        box = {
            // positions
            -1.0f, 1.0f,  -1.0f,  // 0
            -1.0f, -1.0f, -1.0f,  // 1
            1.0f,  -1.0f, -1.0f,  // 2
            1.0f,  -1.0f, -1.0f,  // 3
            1.0f,  1.0f,  -1.0f,  // 4
            -1.0f, 1.0f,  -1.0f,  // 5
            -1.0f, -1.0f, 1.0f,   // 6

            -1.0f, -1.0f, -1.0f,  // 7
            -1.0f, 1.0f,  -1.0f,  // 8
            -1.0f, 1.0f,  -1.0f,  // 9
            -1.0f, 1.0f,  1.0f,   // 10
            -1.0f, -1.0f, 1.0f,   // 11
            1.0f,  -1.0f, -1.0f,  // 12

            1.0f,  -1.0f, 1.0f,   // 13
            1.0f,  1.0f,  1.0f,   // 14
            1.0f,  1.0f,  1.0f,   // 15
            1.0f,  1.0f,  -1.0f,  // 16
            1.0f,  -1.0f, -1.0f,  // 17
            -1.0f, -1.0f, 1.0f,   // 18

            -1.0f, 1.0f,  1.0f,   // 19
            1.0f,  1.0f,  1.0f,   // 20
            1.0f,  1.0f,  1.0f,   // 21
            1.0f,  -1.0f, 1.0f,   // 22
            -1.0f, -1.0f, 1.0f,   // 23
            -1.0f, 1.0f,  -1.0f,  // 24

            1.0f,  1.0f,  -1.0f,  // 25
            1.0f,  1.0f,  1.0f,   // 26
            1.0f,  1.0f,  1.0f,   // 27
            -1.0f, 1.0f,  1.0f,   // 28
            -1.0f, 1.0f,  -1.0f,  // 29

            -1.0f, -1.0f, -1.0f,  // 30
            -1.0f, -1.0f, 1.0f,   // 31
            1.0f,  -1.0f, -1.0f,  // 32
            1.0f,  -1.0f, -1.0f,  // 33
            -1.0f, -1.0f, 1.0f,   // 34
            1.0f,  -1.0f, 1.0f    // 35
        };

    _skyboxBuffer.As<gl::BufferTarget::ARRAY_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::ARRAY_BUFFER>& buf) {
          buf.Reserve(sizeof(float32) * box.size(), box.data(), GL_STATIC_DRAW);
          _skyboxLayout.As<gl::LayoutTarget::VERTEX_ARRAY>(
              [&](gl::LayoutView<gl::LayoutTarget::VERTEX_ARRAY>& layout) {
                layout.EnableAttribute(0);
                layout.SetAttribute(0, 3, GL_FLOAT, 0, 0);
              });
        });

    _skyboxShader =
        Shader("shaders/SkyboxVertex.shader", "shaders/SkyboxFragment.shader");

    _cameraBuffer.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER>& self) {
          auto matsz = sizeof(glm::mat4);
          self.Reserve(matsz * 2, nullptr, GL_STATIC_DRAW);
          self.Write(0, matsz, glm::value_ptr(_camera.GetProjection()));
          self.BindBase(0);
        });

    _baseShader.BindUniform("Camera", 0);
    _skyboxShader.BindUniform("Camera", 0);

    _debugShader = Shader("shaders/NormalDebugVertex.shader",
                          "shaders/NormalDebugFragment.shader",
                          "shaders/NormalDebugGeometry.shader");
  }

  void Update(float32 dt) override {
    if (Input::Instance().IsPressed(Input::Key::ESCAPE)) {
      _window.SetShouldClose(true);
    }

    _camera.SetSpeed(25.f);
    if (Input::Instance().IsPressed(Input::Key::LEFT_CONTROL)) {
      _camera.SetSpeed(5.f);
    }

    if (Input::Instance().IsPressed(Input::Key::E)) {
      _explode += dt;
    }
    if (Input::Instance().IsPressed(Input::Key::R)) {
      _explode = 0.f;
    }

    float32 rotationSpeed = glm::radians(0.25f);

    if (Input::Instance().IsPressed(Input::Key::Y)) {
      _model->GetTransform().rotation.x += rotationSpeed;
    }

    if (Input::Instance().IsPressed(Input::Key::H)) {
      _model->GetTransform().rotation.x -= rotationSpeed;
    }

    if (Input::Instance().IsPressed(Input::Key::J)) {
      _model->GetTransform().rotation.y += rotationSpeed;
    }

    if (Input::Instance().IsPressed(Input::Key::G)) {
      _model->GetTransform().rotation.y -= rotationSpeed;
    }

    if (Input::Instance().IsPressed(Input::Key::N)) {
      _model->GetTransform().rotation.z += rotationSpeed;
    }

    if (Input::Instance().IsPressed(Input::Key::B)) {
      _model->GetTransform().rotation.z -= rotationSpeed;
    }

    _inverted = Input::Instance().IsPressed(Input::Key::LEFT_SHIFT);
    _reflectFlag = Input::Instance().IsPressed(Input::Key::Q);
    _debug = Input::Instance().IsPressed(Input::Key::TAB);

    _camera.UpdatePositionCallback(_window.Get(), dt);
    auto [xpos, ypos] = Input::Instance().GetCursorPosition();
    _camera.UpdateYawPitchCallback(xpos, ypos);

    _cameraBuffer.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER>& self) {
          auto matsz = sizeof(glm::mat4);
          self.Write(matsz, matsz, glm::value_ptr(_camera.GetView()));
        });

    _frame.As<gl::FrameBufferTarget::FRAMEBUFFER>([&]() {
      _ctx.SetClearColor({0.f, 0.5f, 0.5f, 1.f});

      // Because _ctx.ClearAll() only cleans enabled tests
      _ctx.SetFaceCulling(true);
      _ctx.SetDepthTest(true);
      _ctx.SetStencilTest(true);

      _ctx.ClearAll();

      _ctx.SetFaceCulling(false);
      _ctx.SetDepthTest(false);
      _ctx.SetStencilTest(false);

      // Model rendering
      _baseShader.Use([&] {
        _ctx.SetFaceCulling(true);
        _ctx.SetDepthTest(true);

        _baseShader.SetMatrix4f("model", _model->GetTransform().GetModel());
        _baseShader.SetVec3f("cameraPosition", _camera.GetPosition());

        gl::Texture::Activate(GL_TEXTURE0 + 2);
        _baseShader.SetInt("skybox", 2);
        _baseShader.SetBool("doReflect", _reflectFlag);
        _baseShader.SetFloat("time", _explode);
        _cubeMap.As<gl::TextureTarget::TEXTURE_CUBE_MAP>(
            [&] { _model->Draw(); });
      });

      // Skybox rendering (after model for optimization)
      _skyboxShader.Use([&] {
        _ctx.SetDepthTest(true);
        glDepthFunc(GL_LEQUAL);

        _skyboxLayout.As<gl::LayoutTarget::VERTEX_ARRAY>([&] {
          _cubeMap.As<gl::TextureTarget::TEXTURE_CUBE_MAP>(
              [&] { glDrawArrays(GL_TRIANGLES, 0, 36); });
        });

        glDepthFunc(GL_LESS);
      });

      if (_debug) {
        _debugShader.Use([&] {
          _debugShader.SetMatrix4f("model", _model->GetTransform().GetModel());
          _debugShader.SetFloat("magnitude", 0.4f);

          _model->Draw();
        });
      }
    });

    // to default screen framebuffer
    _screenShader.Use([&]() {
      _ctx.SetClearColor({1.f, 1.f, 1.f, 1.f});
      _ctx.SetDepthTest(false);
      _ctx.ClearAll();

      _screenShader.SetBool("material.inverted", _inverted);

      _quad.Draw();
    });

    _elapsedTime += dt;
    if (_elapsedTime >= 1.f) {
      _elapsedTime = 0.f;
      fmt::println("fps: {}", _fps);
    }

    auto [width, height] = _window.GetSize();

    _camera.SetAspectRatio(width * 1.0 / height);
    _color.As<gl::AsTexture2D>([&](gl::Texture2DView& self) {
      self.Reserve2D(GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
      self.GenerateMipmap();

      self.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      self.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    });

    _rbuffer.As<gl::RenderBufferTarget::RENDER_BUFFER>(
        [&](gl::RenderBufferView<gl::RenderBufferTarget::RENDER_BUFFER>& self) {
          self.Reserve(GL_DEPTH24_STENCIL8, width, height);
        });
  }

 private:
  Shader _baseShader;
  Shader _screenShader;
  Shader _skyboxShader;
  Shader _debugShader;

  gl::BufferWrapper<> _skyboxBuffer;
  gl::LayoutWrapper<> _skyboxLayout;

  gl::FrameBufferWrapper<> _frame;

  gl::TextureWrapper<> _color;

  gl::TextureWrapper<> _cubeMap;
  std::array<std::string, 6> _cubeMapTexturesNames;

  gl::RenderBufferWrapper<> _rbuffer;
  Mesh _quad;
  std::unique_ptr<Model> _model;

  Camera _camera;
  gl::BufferWrapper<> _cameraBuffer;

  float32 _elapsedTime;

  bool _inverted = false;
  bool _reflectFlag = false;
  float32 _explode;
  bool _debug;
};

void Run() {
  FeaturesApp app;
  app.Run(1980, 960);
}

}  // namespace over
