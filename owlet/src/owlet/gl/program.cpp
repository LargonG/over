#include <owlet/gl/program.h>

#include <glad/gl.h>

#include <owlet/debug/core.h>
#include <owlet/debug/gl.h>
#include <owlet/gl/memory.h>
#include <owlet/gl/shader.h>

namespace owlet::gl {
Program::Program(Context* gl, ProgramAllocator* alloc)
    : Object(gl, [](Context* gl) { return gl->DefaultProgramAllocator(); }, alloc) {}

Program& Program::Attach(VertexShader& shader) {
    Attach(static_cast<Shader&>(shader));

    m_vertex_part.push_back(&shader);

    return *this;
}

Program& Program::Attach(FragmentShader& shader) {
    Attach(static_cast<Shader&>(shader));

    m_fragment_part.push_back(&shader);

    return *this;
}

void Program::Link() {
    auto* gl = GL();

    gl->LinkProgram(RawId());
}

void Program::Attach(Shader& shader) {
    auto* gl = GL();

    gl->AttachShader(RawId(), static_cast<GLuint>(shader.Id()));
    debug::GLCheckError(gl);
}

GLint Program::Parameter(Context* gl, ProgramId id, GLenum param) {
    auto raw_id = static_cast<GLuint>(id);

    GLint result = 0;
    gl->GetProgramiv(raw_id, param, &result);
    debug::GLCheckError(gl);

    return result;
}

ProgramId SimpleProgramAllocator::Alloc(Context* gl) {
    GLuint result = gl->CreateProgram();
    debug::GLCheckError(gl);

    return ProgramId(result);
}

void SimpleProgramAllocator::Dealloc(Context* gl, ProgramId id) noexcept {
    auto raw_id = static_cast<GLuint>(id);
    debug::Assert(gl->IsProgram(raw_id));

    gl->DeleteProgram(raw_id);
    debug::GLCheckError(gl);
}

}    // namespace owlet::gl
