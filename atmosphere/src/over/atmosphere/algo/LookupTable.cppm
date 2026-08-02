module;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <over/core/Mesh.hpp>
#include <over/core/Shader.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/targets/FrameBufferTarget.hpp>
#include <over/core/opengl/targets/TextureTarget.hpp>
#include <over/core/opengl/views/FrameBufferView.hpp>
#include <over/core/opengl/views/TextureView.hpp>
#include <over/core/opengl/wrappers/FrameBufferWrapper.hpp>
#include <over/core/opengl/wrappers/TextureWrapper.hpp>
#include <over/core/window/Context.hpp>
#include <over/engine/App.hpp>

export module lookup;

export namespace over {
class LookupTable {
 public:
  // problem: for each class, whose obj we create in App we have to create default ctor and move ctor/operator=
  LookupTable() noexcept : LookupTable(0.f, 0.f, 0.f, 0, 0) {}

  LookupTable(float32 rayleighH, float32 mieH, float32 atmosphereRadius,
              int32 rayleighSamples, int32 mieSamples) noexcept
      : _values(),
        _frame(),
        _ubo(),
        _calcShader(),
        _quad(),
        _rayleighH(rayleighH),
        _mieH(mieH),
        _atmosphereRadius(atmosphereRadius),
        _rayleighSamples(rayleighSamples),
        _mieSamples(mieSamples) {}

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
          self.Reserve(sizeof(glm::ivec2) * 2 + sizeof(glm::vec2) * 2, nullptr,
                       GL_STATIC_DRAW);
          self.Write(sizeof(glm::ivec2), sizeof(glm::vec2),
                     glm::value_ptr(glm::vec2(_rayleighH, _mieH)));
          self.Write(sizeof(glm::ivec2) + sizeof(glm::vec2), sizeof(glm::vec2),
                     glm::value_ptr(glm::vec2(1.f, _atmosphereRadius)));
          self.Write(sizeof(glm::ivec2) + sizeof(glm::vec2) * 2,
                     sizeof(glm::ivec2),
                     glm::value_ptr(glm::ivec2(_rayleighSamples, _mieSamples)));
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
          self.Write(0, sizeof(glm::ivec2),
                     glm::value_ptr(glm::ivec2(heightSamples, angleSamples)));
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

  float32 _rayleighH;
  float32 _mieH;
  float32 _atmosphereRadius;

  int32 _rayleighSamples;
  int32 _mieSamples;
};
}  // namespace over
