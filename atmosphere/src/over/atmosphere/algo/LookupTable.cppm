module;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstddef>

#include <over/core/Mesh.hpp>
#include <over/core/Shader.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/GL.hpp>
#include <over/core/window/Context.hpp>
#include <over/engine/App.hpp>

export module lookup;

export namespace over {
class LookupTable {
 private:
  struct Args {
    glm::ivec2 samples;
    glm::ivec2 size;
    glm::vec2 h0;
    float32 scaleFactor;
  } _args;

  struct Scatter {
    int32 samples;
    float32 h0;
  };

 public:
  // problem: for each class, whose obj we create in App we have to create default ctor and move ctor/operator=
  LookupTable() noexcept : LookupTable(0.f, 0.f, 0.f, 0, 0) {}

  LookupTable(float32 rayleighH, float32 mieH, float32 scaleFactor,
              int32 rayleighSamples, int32 mieSamples) noexcept
      : _values(),
        _frame(),
        _ubo(),
        _calcShader(),
        _quad(),
        _rayleigh({rayleighSamples, rayleighH}),
        _mie({mieSamples, mieH}),
        _scaleFactor(scaleFactor) {}

  LookupTable(LookupTable&&) noexcept = default;
  LookupTable& operator=(LookupTable&&) noexcept = default;

  void Init(int32 width, int32 height) {
    _values.As<gl::TextureTarget::TEXTURE_2D>(
        [&](gl::TextureView<gl::TextureTarget::TEXTURE_2D> self) {
          self.Reserve2D(GL_RG32F, width, height, GL_RGB, GL_FLOAT, nullptr);

          self.SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          self.SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

          self.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          self.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        });

    _frame.As<gl::FrameBufferTarget::FRAMEBUFFER>(
        [&](gl::FrameBufferView<gl::FrameBufferTarget::FRAMEBUFFER> self) {
          self.Attach(GL_COLOR_ATTACHMENT0,
                      _values.As<gl::TextureTarget::TEXTURE_2D>(), 0);

          self.ReadyOrThrow("Cannot create Lookup Table");
        });

    _ubo.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
          self.Reserve(sizeof(_args), nullptr, GL_STATIC_DRAW);

          self.Write(
              offsetof(Args, Args::samples), sizeof(Args::samples),
              glm::value_ptr(glm::ivec2(_rayleigh.samples, _mie.samples)));
          self.Write(offsetof(Args, Args::h0), sizeof(Args::h0),
                     glm::value_ptr(glm::vec2(_rayleigh.h0, _mie.h0)));

          self.Write(offsetof(Args, Args::scaleFactor),
                     sizeof(Args::scaleFactor), &_scaleFactor);

          self.BindBase(1);
        });

    _calcShader =
        Shader("shaders/LookupTable.vert", "shaders/LookupTable.frag");

    _quad = Mesh::GenQuad({});
  }

  void Calculate(int32 heightSamples, int32 angleSamples) {
    Init(heightSamples, angleSamples);
    _ubo.As<gl::BufferTarget::UNIFORM_BUFFER>(
        [&](gl::BufferView<gl::BufferTarget::UNIFORM_BUFFER> self) {
          glm::ivec2 size = glm::ivec2(heightSamples, angleSamples);
          self.Write(offsetof(Args, Args::size), sizeof(Args::size),
                     glm::value_ptr(size));
        });

    _calcShader.BindUniform("LookupTableArguments", 1);
    _frame.As<gl::FrameBufferTarget::FRAMEBUFFER>([&] {
      _calcShader.Use([&] {
        auto& ctx = App::Instance().GetContext();
        ctx.Viewport(0, 0, heightSamples, angleSamples);
        ctx.SetDepthTest(false);
        ctx.SetFaceCulling(false);
        _quad.Draw();
      });
    });
  }

  gl::TextureView<gl::TextureTarget::TEXTURE_2D> Values() {
    return _values.As<gl::TextureTarget::TEXTURE_2D>();
  }

 private:
  gl::TextureWrapper<> _values;
  gl::FrameBufferWrapper<> _frame;

  gl::BufferWrapper<> _ubo;

  Shader _calcShader;

  Mesh _quad;

  Scatter _rayleigh;
  Scatter _mie;

  float32 _scaleFactor;
};
}  // namespace over
