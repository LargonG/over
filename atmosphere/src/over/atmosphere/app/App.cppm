module;

#include <glm/gtc/type_ptr.hpp>
#include <over/core/Camera.hpp>
#include <over/core/Shader.hpp>
#include <over/core/opengl/views/BufferView.hpp>
#include <over/core/opengl/wrappers/BufferWrapper.hpp>
#include <over/engine/App.hpp>

export module app;

import shapes;

namespace over {
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
        _camera({0, 0, 0}, {0, 0, 0}, .25f, 45.f, 16.f / 9.f, 0.001f),
        _planet(),
        _ubo(),
        _elapsedTime(0) {}

  void Init() override {
    PrintName();

    _input.SetCursor(false);

    // TODO
    // - Create 2 spheres, one - planet, another - sky dome (inverted sphere)
    // - Of course use gamma correction & multisampling
    // - Implement camera space shader
    // - Implement camera atmosphere shader
    // - Profit

    _ubo.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
          self.Reserve(sizeof(glm::mat4) * 3, nullptr, GL_STATIC_DRAW);
          self.Write(0, sizeof(glm::mat4),
                     glm::value_ptr(_camera.GetProjection()));
          self.Write(sizeof(glm::mat4) * 2, sizeof(glm::mat4),
                     glm::value_ptr(glm::mat4(1.f)));
          self.BindBase(0);
        });

    _planetShader = Shader("shaders/Planet.vert", "shaders/Planet.frag");
    _planetShader.BindUniform("Camera", 0);

    _planet = Sphere(n);
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

    _ubo.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
          self.Write(sizeof(glm::mat4), sizeof(glm::mat4),
                     glm::value_ptr(_camera.GetView()));
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
        glDrawArrays(GL_POINTS, 0, _planet.VerticesCount());

        glDrawElements(GL_TRIANGLES, _planet.ElementsCount() * 3,
                       GL_UNSIGNED_INT, nullptr);
      });
    });

    _elapsedTime += dt;

    if (_elapsedTime > 0.5f) {
      _elapsedTime -= 0.5f;
      n += 1;
      _planet = Sphere(n);
    }
  }

  Shader _planetShader;

  Camera _camera;

  float32 _elapsedTime;

  gl::BufferWrapper<> _ubo;
  Sphere _planet;
  uint32 n = 1;
};
}  // namespace over
