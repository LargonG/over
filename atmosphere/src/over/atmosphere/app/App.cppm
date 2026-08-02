module;

#include <glm/gtc/type_ptr.hpp>
#include <over/core/Camera.hpp>
#include <over/core/Mesh.hpp>
#include <over/core/Shader.hpp>
#include <over/core/Types.hpp>

#include <over/core/opengl/Texture.hpp>
#include <over/core/opengl/targets/TextureTarget.hpp>
#include <over/core/opengl/views/BufferView.hpp>
#include <over/core/opengl/views/TextureView.hpp>
#include <over/core/opengl/wrappers/BufferWrapper.hpp>
#include <over/core/opengl/wrappers/FrameBufferWrapper.hpp>
#include <over/core/opengl/wrappers/RenderBufferWrapper.hpp>
#include <over/core/opengl/wrappers/TextureWrapper.hpp>

#include <cstddef>
#include <over/engine/App.hpp>

export module app;

import shapes;
import lookup;

namespace over {

namespace detail {
struct CameraPos {
  glm::vec4 position;
};

struct Light {
  glm::vec4 direction;
  glm::vec4 light;
};

struct Args {
  glm::vec4 rayleigh_kernel;
  glm::vec4 mie_kernel;
  glm::vec2 radius;
  glm::vec2 h0;
  glm::vec2 g;
  glm::ivec2 samples;
  float32 pi;
};

}  // namespace detail

export class AtmosphereApp final : public over::App {
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
        _planetShader(),
        _camera({0, 0, 3}, {0, 0, 0}, 1.25f, 45.f, 16.f / 9.f, 0.001f),
        _planet(),
        _atmosphere(),
        _ubo(),
        _cameraData(),
        _sunData(),
        _argsData(),
        _elapsedTime(0.f) {}

  void Init() override {
    PrintName();

    _input.SetCursor(false);

    // Precalculate lookup table
    _table = LookupTable(_rayleighH0, _mieH0, _skyRadius, 500, 500);
    _table.Calculate(1000, 1800);

    // TODO
    // - Create 2 spheres, one - planet, another - sky dome (inverted sphere) [DONE]
    // - Of course use gamma correction & multisampling [DONE - multisampling]
    // - Implement camera space shader
    // - Implement camera atmosphere shader
    // - Profit

    _ubo.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
          self.Reserve(sizeof(glm::mat4) * 3, nullptr, GL_DYNAMIC_DRAW);
          self.Write(0, sizeof(glm::mat4),
                     glm::value_ptr(_camera.GetProjection()));
          self.Write(sizeof(glm::mat4) * 2, sizeof(glm::mat4),
                     glm::value_ptr(glm::mat4(1.f)));
          self.BindBase(0);
        });

    using detail::Args;
    using detail::CameraPos;
    using detail::Light;

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

    _planetShader = Shader("shaders/Planet.vert", "shaders/Planet.frag");
    _planetShader.BindUniform("Camera", 0);

    _skyShader =
        Shader("shaders/SkyFromSpace.vert", "shaders/SkyFromSpace.frag");
    _skyShader.BindUniform("World", 0);
    _skyShader.BindUniform("Camera", 1);
    _skyShader.BindUniform("Light", 2);
    _skyShader.BindUniform("Args", 3);

    _planet = Sphere(n, false);

    _atmosphere = Sphere(m, true);

    _cameraData = {.position = glm::vec4(_camera.GetPosition(), 0)};
    _sunData = {.direction = glm::vec4(0, -1, 1, 0),
                .light = glm::vec4(1.0, 1.0, 1.0, 0.0)};

    _argsData = {.rayleigh_kernel = {.3f, .4f, 1.f, 0.f},
                 .mie_kernel = {1.f, .01f, .01f, 0.f},
                 .radius = {_planetRadius, _skyRadius},
                 .h0 = {_rayleighH0, _mieH0},
                 .g = {0.f, -0.75f},

                 .samples = {10, 5},
                 .pi = glm::pi<float32>()};
  }

  void Update(float32 dt) override {
    if (_input.IsPressed(Input::Key::ESCAPE)) {
      _window.SetShouldClose(true);
      return;
    }

    bool shift = _input.IsPressed(Input::Key::LEFT_SHIFT);

    _camera.UpdatePositionCallback(_window.Get(), dt);
    auto [xpos, ypos] = Input::Instance().GetCursorPosition();
    _camera.UpdateYawPitchCallback(xpos, ypos);

    if (glm::length(_camera.GetPosition()) < _planetRadius + 0.01f) {
      _camera.GetPosition() =
          glm::normalize(_camera.GetPosition()) * (_planetRadius + 0.01f);
    }

    _ubo.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
          self.Write(sizeof(glm::mat4), sizeof(glm::mat4),
                     glm::value_ptr(_camera.GetView()));
        });

    _cameraData.position = glm::vec4(_camera.GetPosition(), 0.f);

    using detail::Args;
    using detail::CameraPos;
    using detail::Light;

    _cameraBuffer.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
          self.Write(0, sizeof(CameraPos), &_cameraData);
        });

    _sunBuffer.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
          self.Write(0, sizeof(Light), &_sunData);
        });

    _argsBuffer.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
          self.Write(0, sizeof(Args), &_argsData);
        });

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
        _ubo.As<gl::BufferTarget::UNIFORM_BUFFER>(
            [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
              self.Write(sizeof(glm::mat4) * 2, sizeof(glm::mat4),
                         glm::value_ptr(glm::mat4(1.f)));
            });

        glDrawElements(GL_TRIANGLES, _planet.ElementsCount() * 3,
                       GL_UNSIGNED_INT, nullptr);
      });
    });

    _skyShader.Use([&] {
      _atmosphere.Layout().Use([&] {
        _ubo.As<gl::BufferTarget::UNIFORM_BUFFER>(
            [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
              auto m = glm::mat4(1.f);
              m = glm::scale(m, glm::vec3(1.19f));

              self.Write(sizeof(glm::mat4) * 2, sizeof(glm::mat4),
                         glm::value_ptr(m));
            });

        gl::Texture::Activate(GL_TEXTURE0);
        _skyShader.SetInt("lookup", 0);
        _table.Values().Use([&] {
          glDrawElements(GL_TRIANGLES, _atmosphere.ElementsCount() * 3,
                         GL_UNSIGNED_INT, nullptr);
        });
      });
    });

    _elapsedTime += dt;

    if (_elapsedTime >= 1.f) {
      _elapsedTime -= 1.f;
      fmt::println("fps: {}", _fps);
    }
  }

  float32 _elapsedTime;

  Shader _planetShader;
  Shader _skyShader;

  float32 _planetRadius = 1.f;
  float32 _skyRadius = 1.19f;

  float32 _rayleighH0 = 0.25f;
  float32 _mieH0 = 0.12f;

  Camera _camera;

  gl::BufferWrapper<> _ubo;
  gl::BufferWrapper<> _cameraBuffer;
  gl::BufferWrapper<> _sunBuffer;
  gl::BufferWrapper<> _argsBuffer;

  detail::CameraPos _cameraData;
  detail::Light _sunData;
  detail::Args _argsData;

  Sphere _planet;
  Sphere _atmosphere;

  LookupTable _table;

  uint32 n = 200;
  uint32 m = 3000;
};
}  // namespace over
