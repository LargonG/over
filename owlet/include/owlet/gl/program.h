#pragma once

#include <owlet/gl/memory.h>

#include <span>
#include <string>
#include <vector>

namespace owlet::gl {

struct Shader;
struct VertexShader;
struct FragmentShader;

WL_DEF_ALLOC_IMPL(Program);

struct SimpleProgramAllocator : ProgramAllocator {
    // Inherited via ProgramAllocator
    ProgramId Alloc(Context*) override;
    void Dealloc(Context*, ProgramId) noexcept override;
};

struct UniformVar {
    int32 location = -1;
    GLenum type = GL_NONE;
    std::string name = "";
};

struct Program : Object<ProgramId> {
  public:
    enum class Parameter : GLenum {
        DeleteStatus = GL_DELETE_STATUS,
        InfoLogLength = GL_INFO_LOG_LENGTH,
        ActiveUniforms = GL_ACTIVE_UNIFORMS,
        ActiveUniformMaxLength = GL_ACTIVE_UNIFORM_MAX_LENGTH,
    };

    explicit Program(Context* gl, ProgramAllocator* alloc = nullptr);

    Program& Attach(VertexShader&);
    Program& Attach(FragmentShader&);

    void Link();

    GLint GetParameter(Parameter param) { return Parameter(m_handler.Context(), m_handler.Get(), param); }

    [[nodiscard]] std::span<VertexShader* const> VertexSources() const noexcept { return m_vertex_part; }
    [[nodiscard]] std::span<FragmentShader* const> FragmentSources() const noexcept { return m_fragment_part; }

    std::vector<UniformVar> ActiveUniforms();
    int32 UniformLocation(const std::string& name);

    Program& UpdateUniform(int32 location, float x);
    Program& UpdateUniform(int32 location, float x, float y);
    Program& UpdateUniform(int32 location, float x, float y, float z);
    Program& UpdateUniform(int32 location, float x, float y, float z, float w);
    Program& UpdateUniform(int32 location, int32 count, int32 size, const float* data);

    Program& UpdateUniform(int32 location, int32 a);
    Program& UpdateUniform(int32 location, int32 a, int32 b);
    Program& UpdateUniform(int32 location, int32 a, int32 b, int32 c);
    Program& UpdateUniform(int32 location, int32 a, int32 b, int32 c, int32 d);
    Program& UpdateUniform(int32 location, int32 count, int32 size, const int32* data);

    // GLuint is NOT SUPPORTED

    void Use();

  private:
    void Attach(Shader&);

    std::vector<VertexShader*> m_vertex_part;
    std::vector<FragmentShader*> m_fragment_part;

    static GLint Parameter(Context* gl, ProgramId id, Parameter param) {
        return Parameter(gl, id, static_cast<GLenum>(param));
    }
    static GLint Parameter(Context*, ProgramId, GLenum param);
};
}    // namespace owlet::gl
