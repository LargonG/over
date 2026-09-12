#pragma once

#include <owlet/gl/memory.h>

#include <span>
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

struct Program : Object<ProgramId> {
  public:
    enum class Parameter : GLenum {
        DeleteStatus = GL_DELETE_STATUS,
        InfoLogLength = GL_INFO_LOG_LENGTH,
    };

    explicit Program(Context* gl, ProgramAllocator* alloc = nullptr);

    Program& Attach(VertexShader&);
    Program& Attach(FragmentShader&);

    void Link();

    [[nodiscard]] std::span<VertexShader* const> VertexSources() const noexcept { return m_vertex_part; }
    [[nodiscard]] std::span<FragmentShader* const> FragmentSources() const noexcept { return m_fragment_part; }

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
