#pragma once

#include <span>
#include <string>
#include <string_view>

#include <owlet/gl/memory.h>
#include <vector>

namespace owlet::gl {

enum class ShaderId : GLuint { Null = 0 };

struct ShaderAllocator : Allocator<ShaderId> {
  public:
    virtual GLenum ShaderType();
};

struct SimpleShaderAllocator : ShaderAllocator {
  public:
    explicit SimpleShaderAllocator(GLenum shader_type);

    // Inherited via ShaderAllocator
    ShaderId Alloc(Context*) override;
    void Dealloc(Context*, ShaderId) noexcept override;

  private:
    GLenum m_shader_type;
};

struct Shader : Object<ShaderId> {
  public:
    enum Parameter : GLenum {
        Type = GL_SHADER_TYPE,
        DeleteStatus = GL_DELETE_STATUS,
        CompileStatus = GL_COMPILE_STATUS,
        InfoLogLength = GL_INFO_LOG_LENGTH,
        ShaderSourceLength = GL_SHADER_SOURCE_LENGTH
    };

    static GLint GetParameter(Context* gl, ShaderId id, Shader::Parameter param);

  public:
    void Sources(std::span<std::string> sources);

    std::span<std::string_view> Sources();

    bool Compile();
    bool IsCompiled();
    std::string InfoLog();

  protected:
    explicit Shader(GLenum type, Context* gl, ShaderAllocator* alloc = nullptr);

    std::vector<std::string> m_sources;
    GLenum m_type;
};

struct VertexShader : Shader {
    explicit VertexShader(Context* gl, ShaderAllocator* alloc = nullptr);
};

struct FragmentShader : Shader {
    explicit FragmentShader(Context* gl, ShaderAllocator* alloc = nullptr);
};
}    // namespace owlet::gl
