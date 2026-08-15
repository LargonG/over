module;
#include <cstddef>

#include <glm/gtc/type_ptr.hpp>

#include <over/core/Camera.hpp>
#include <over/core/Mesh.hpp>
#include <over/core/Shader.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/GL.hpp>

#include <over/engine/App.hpp>

export module app;

import shapes;
import lookup;

namespace over {
export class AtmosphereApp final : public over::App {
 private:
  struct WorldMatrix {
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
  } _worldData;

  struct CameraPos {
    glm::vec4 position;
  } _cameraData;

  struct Light {
    glm::vec4 direction;
    glm::vec4 light;
  } _lightData;

  struct Args {
    glm::ivec2 samples;
    glm::vec2 h0;
    glm::vec2 g;
    float32 scaleFactor;
  } _argsData;

  struct Kernels {
    glm::vec4 rayleigh;
    glm::vec4 mie;
  } _kernelsData;

 public:
  // out-scattering - use for sample point (how much light scattered)
  // in-scattering - use for sample point to camera (how much light camera get from sample point)
  //
  // out-scattering(from, to, wavelength) = 4 * pi * K(wavelength) * [integral<from, to>(- h / H_0)ds = "optical depth"]
  //
  //
  // 2D lookup table
  // x - altitude (height)
  // y - vertical angle (0.0 - up, 1.0 - down)
  // tb[x, y] = (rayleigh_density = exp(- h/H0), optical depth)

  AtmosphereApp()
      : App("atmosphere"),
        _cameraData(),
        _lightData(),
        _argsData(),
        _kernelsData(),
        _planetShader(),
        _camera({0, 0, _planetRadius + (_skyRadius - _planetRadius) * (0.01)},
                {0, 0, 0}, 250.f, 45.f, 16.f / 9.f, 0.001f),
        _planet(),
        _atmosphere(),
        worldBuffer(),
        _elapsedTime(0.f),
        _ctrlUp(true) {}

  void Init() override {
    PrintName();

    glm::vec2 h0 = {0.25, 0.12};

    _input.SetCursor(false);

    // Precalculate lookup table
    _table = LookupTable(h0.x, h0.y, _skyRadius / _planetRadius, 500, 500);
    _table.Calculate(1000, 1000);

    // TODO
    // - Create 2 spheres, one - planet, another - sky dome (inverted sphere) [DONE]
    // - Of course use gamma correction & multisampling [DONE - multisampling]
    // - Implement camera space shader [DONE]
    // - Implement camera atmosphere shader [DONE]
    // - Profit

    _worldData = {
        .projection = _camera.GetProjection(),
        .view = _camera.GetView(),
        .model = glm::mat4(1.0),
    };

    worldBuffer.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::UniformBuffer self) {
          self.Reserve(sizeof(WorldMatrix), &_worldData, GL_DYNAMIC_DRAW);

          self.BindBase(0);
        });

    _cameraBuffer.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
          self.Reserve(sizeof(CameraPos), nullptr, GL_DYNAMIC_DRAW);
          self.BindBase(1);
        });

    _sunBuffer.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
          self.Reserve(sizeof(Light), nullptr, GL_DYNAMIC_DRAW);
          self.BindBase(2);
        });

    _argsBuffer.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
          self.Reserve(sizeof(Args), nullptr, GL_DYNAMIC_DRAW);
          self.BindBase(3);
        });

    _kernelsBuffer.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
          self.Reserve(sizeof(Kernels), nullptr, GL_DYNAMIC_DRAW);
          self.BindBase(4);
        });

    _planetShader = Shader("shaders/Planet.vert", "shaders/Planet.frag");
    _planetShader.BindUniform("Camera", 0);

    _skyShader =
        Shader("shaders/SkyFromSpace.vert", "shaders/SkyFromSpace.frag");
    _skyShader.BindUniform("World", 0);
    _skyShader.BindUniform("Camera", 1);
    _skyShader.BindUniform("Light", 2);
    _skyShader.BindUniform("Args", 3);
    _skyShader.BindUniform("Kernels", 4);

    _frameShader = Shader("shaders/Frame.vert", "shaders/Frame.frag");

    _planet = Sphere(n, false);

    _atmosphere = Sphere(m, true);

    _cameraData = {
        .position = glm::vec4(_camera.GetPosition() / _planetRadius, 0),
    };

    _lightData = {
        .direction = glm::vec4(0, 0, 1, 0),
        .light = glm::vec4(1.8, 1.8, 1.8, 0.0),
    };

    _argsData = {
        .samples = {30, 30},
        .h0 = h0,
        .g = {0.f, -0.9f},
        .scaleFactor = _skyRadius / _planetRadius,
    };

    _kernelsData = {
        .rayleigh = {.025f, .25f, .75f, 0},
        .mie = {0.02, 0.02, 0.02, 0},
    };

    _quad = Mesh::GenQuad(
        {MeshTexture(_table.Values(), MeshTexture::Type::DIFFUSE)});

    _window.SetSwapInterval(1);
  }

  void Update(float32 dt) override {
    if (_input.IsPressed(Input::Key::ESCAPE)) {
      _window.SetShouldClose(true);
      return;
    }

    bool shift = _input.IsPressed(Input::Key::LEFT_SHIFT);

    auto ctrl = _input.IsPressed(Input::Key::LEFT_CONTROL);

    if (ctrl && _ctrlUp) {
      _show = !_show;
    }

    _camera.UpdatePositionCallback(_window.Get(), dt);
    auto [xpos, ypos] = Input::Instance().GetCursorPosition();
    _camera.UpdateYawPitchCallback(xpos, ypos);

    _worldData.view = _camera.GetView();
    _cameraData.position =
        glm::vec4(_camera.GetPosition() / _planetRadius, 0.f);

    DEF_UPDATE_UNIFORM_BUFFER(worldBuffer, _worldData);
    DEF_UPDATE_UNIFORM_BUFFER(_cameraBuffer, _cameraData);
    DEF_UPDATE_UNIFORM_BUFFER(_sunBuffer, _lightData);
    DEF_UPDATE_UNIFORM_BUFFER(_argsBuffer, _argsData);
    DEF_UPDATE_UNIFORM_BUFFER(_kernelsBuffer, _kernelsData);

    glEnable(GL_PROGRAM_POINT_SIZE);
    if (shift) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    _planetShader.Use([&] {
      _ctx.SetDepthTest(true);
      _ctx.SetFaceCulling(true);

      _planet.Layout().Use([&] {
        _worldData.model = glm::scale(glm::mat4(1.0), glm::vec3(_planetRadius));
        DEF_UPDATE_UNIFORM_BUFFER(worldBuffer, _worldData);

        glDrawElements(GL_TRIANGLES, _planet.ElementsCount() * 3,
                       GL_UNSIGNED_INT, nullptr);
      });
    });

    _skyShader.Use([&] {
      _atmosphere.Layout().Use([&] {
        _worldData.model = glm::scale(glm::mat4(1.0), glm::vec3(_skyRadius));
        DEF_UPDATE_UNIFORM_BUFFER(worldBuffer, _worldData);

        gl::Texture::Activate(GL_TEXTURE0);
        _skyShader.SetInt("lookup", 0);
        _table.Values().Use([&] {
          glDrawElements(GL_TRIANGLES, _atmosphere.ElementsCount() * 3,
                         GL_UNSIGNED_INT, nullptr);
        });
      });
    });

    if (_show) {
      _frameShader.Use([&] { _quad.Draw(); });
    }

    _elapsedTime += dt;

    if (_elapsedTime >= 1.f) {
      _elapsedTime -= 1.f;
      fmt::println("fps: {}", _fps);
    }

    _ctrlUp = _input.IsReleased(Input::Key::LEFT_CONTROL);
  }

  float32 _elapsedTime;

  Shader _planetShader;
  Shader _skyShader;
  Shader _frameShader;

  float32 _planetRadius = 100.f;
  float32 _skyRadius = 119.f;

  Camera _camera;

  gl::BufferWrapper<> worldBuffer;
  gl::BufferWrapper<> _cameraBuffer;
  gl::BufferWrapper<> _sunBuffer;
  gl::BufferWrapper<> _argsBuffer;
  gl::BufferWrapper<> _kernelsBuffer;

  Sphere _planet;
  Sphere _atmosphere;

  LookupTable _table;

  uint32 n = 200;
  uint32 m = 200;

  Mesh _quad;

  bool _ctrlUp;
  bool _show = false;
};
}  // namespace over
