#include <owlet/gl/program.h>

#include <iterator>

#include <glad/gl.h>

#include <owlet/debug/core.h>
#include <owlet/debug/gl.h>
#include <owlet/gl/memory.h>
#include <owlet/gl/shader.h>
#include <owlet/types.h>

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

std::vector<UniformVar> Program::ActiveUniforms() {
    auto* gl = GL();

    auto active_uniforms = GetParameter(Parameter::ActiveUniforms);
    auto buffer_size = GetParameter(Parameter::ActiveUniformMaxLength);
    std::vector<UniformVar> result(active_uniforms);
    for (GLint i = 0; i < active_uniforms; i++) {
        std::string name;
        name.resize(buffer_size, 0);

        GLsizei len;
        GLint size;
        GLenum type;

        gl->GetActiveUniform(RawId(), i, buffer_size, &len, &size, &type, static_cast<GLchar*>(&name[0]));
        debug::GLCheckError(gl);

        name.resize(len);

        result[i] = UniformVar{
            .location = UniformLocation(name),
            .type = type,
            .name = name,
        };
    }

    return result;
}

int32 Program::UniformLocation(const std::string& name) {
    auto* gl = GL();

    GLint result = gl->GetUniformLocation(RawId(), name.c_str());
    debug::GLCheckError(gl);

    return static_cast<int32>(result);
}

Program& Program::UpdateUniform(int32 location, float x) {
    auto* gl = GL();

    gl->ProgramUniform1f(RawId(), static_cast<GLint>(location), static_cast<GLfloat>(x));
    debug::GLCheckError(gl);

    return *this;
}

Program& Program::UpdateUniform(int32 location, float x, float y) {
    auto* gl = GL();

    gl->ProgramUniform2f(RawId(), static_cast<GLint>(location), static_cast<GLfloat>(x), static_cast<GLfloat>(y));
    debug::GLCheckError(gl);

    return *this;
}

Program& Program::UpdateUniform(int32 location, float x, float y, float z) {
    auto* gl = GL();

    gl->ProgramUniform3f(RawId(), static_cast<GLint>(location), static_cast<GLfloat>(x), static_cast<GLfloat>(y),
                         static_cast<GLfloat>(z));
    debug::GLCheckError(gl);

    return *this;
}

Program& Program::UpdateUniform(int32 location, float x, float y, float z, float w) {
    auto* gl = GL();

    gl->ProgramUniform4f(RawId(), static_cast<GLint>(location), static_cast<GLfloat>(x), static_cast<GLfloat>(y),
                         static_cast<GLfloat>(z), static_cast<GLfloat>(w));
    debug::GLCheckError(gl);

    return *this;
}

Program& owlet::gl::Program::UpdateUniform(int32 location, int32 count, int32 size, const float* data) {
    auto* gl = GL();

    debug::Assert(0 < size && size < 5);

    auto glocation = static_cast<GLint>(location);
    auto gcount = static_cast<GLint>(count);
    auto gdata = static_cast<const GLfloat*>(data);

    switch (size) {
        case 1:
            gl->ProgramUniform1fv(RawId(), glocation, gcount, gdata);
            break;
        case 2:
            gl->ProgramUniform2fv(RawId(), glocation, gcount, gdata);
            break;
        case 3:
            gl->ProgramUniform3fv(RawId(), glocation, gcount, gdata);
            break;
        case 4:
            gl->ProgramUniform4fv(RawId(), glocation, gcount, gdata);
            break;
    }
    debug::GLCheckError(gl);

    return *this;
}

Program& Program::UpdateUniform(int32 location, int32 a) {
    auto* gl = GL();

    gl->ProgramUniform1i(RawId(), static_cast<GLint>(location), static_cast<GLint>(a));
    debug::GLCheckError(gl);

    return *this;
}

Program& Program::UpdateUniform(int32 location, int32 a, int32 b) {
    auto* gl = GL();

    gl->ProgramUniform2i(RawId(), static_cast<GLint>(location), static_cast<GLint>(a), static_cast<GLint>(b));
    debug::GLCheckError(gl);

    return *this;
}

Program& Program::UpdateUniform(int32 location, int32 a, int32 b, int32 c) {
    auto* gl = GL();

    gl->ProgramUniform3i(RawId(), static_cast<GLint>(location), static_cast<GLint>(a), static_cast<GLint>(b),
                         static_cast<GLint>(c));
    debug::GLCheckError(gl);

    return *this;
}

Program& Program::UpdateUniform(int32 location, int32 a, int32 b, int32 c, int32 d) {
    auto* gl = GL();

    gl->ProgramUniform4i(RawId(), static_cast<GLint>(location), static_cast<GLint>(a), static_cast<GLint>(b),
                         static_cast<GLint>(c), static_cast<GLint>(d));
    debug::GLCheckError(gl);

    return *this;
}

Program& Program::UpdateUniform(int32 location, int32 count, int32 size, const int32* data) {
    auto* gl = GL();

    debug::Assert(0 < size && size < 5);

    auto glocation = static_cast<GLint>(location);
    auto gcount = static_cast<GLint>(count);
    auto gdata = static_cast<const GLint*>(data);

    switch (size) {
        case 1:
            gl->ProgramUniform1iv(RawId(), glocation, gcount, gdata);
            break;
        case 2:
            gl->ProgramUniform2iv(RawId(), glocation, gcount, gdata);
            break;
        case 3:
            gl->ProgramUniform3iv(RawId(), glocation, gcount, gdata);
            break;
        case 4:
            gl->ProgramUniform4iv(RawId(), glocation, gcount, gdata);
            break;
    }
    debug::GLCheckError(gl);

    return *this;
}

void Program::Use() {
    GL()->UseProgram(RawId());
    debug::GLCheckError(GL());
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
