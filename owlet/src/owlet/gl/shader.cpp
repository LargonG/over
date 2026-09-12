#include <owlet/gl/shader.h>

#include <algorithm>
#include <iterator>
#include <string>
#include <tuple>

#include <owlet/debug/core.h>
#include <owlet/debug/gl.h>

#include <fmt/format.h>

namespace owlet::gl {

#pragma region Shader Allocator impl
SimpleShaderAllocator::SimpleShaderAllocator(GLenum shader_type) : m_shader_type(shader_type) {}

ShaderId SimpleShaderAllocator::Alloc(Context* gl) {
    GLuint res = gl->CreateShader(m_shader_type);
    debug::GLCheckError(gl);

    return ShaderId(res);
}

void SimpleShaderAllocator::Dealloc(Context* gl, ShaderId id) noexcept {
    auto raw_id = static_cast<GLuint>(id);
    debug::Assert(gl->IsShader(raw_id) &&
                  static_cast<GLenum>(Shader::GetParameter(gl, id, Shader::Parameter::Type)) == m_shader_type &&
                  static_cast<GLboolean>(Shader::GetParameter(gl, id, Shader::Parameter::DeleteStatus)) == GL_FALSE);

    gl->DeleteShader(raw_id);
    debug::GLCheckError(gl);
}
#pragma endregion

GLint Shader::GetParameter(Context* gl, ShaderId id, Shader::Parameter param) {
    GLint result = 0;

    gl->GetShaderiv(static_cast<GLuint>(id), static_cast<GLenum>(param), &result);
    debug::GLCheckError(gl);

    return result;
}

Shader::Shader(GLenum type, Context* gl, ShaderAllocator* alloc)
    : Object(gl, [](Context* gl) { return gl->DefaultShaderAllocator(); }, alloc), m_type(type), m_sources() {
    debug::Require(
        m_type == alloc->ShaderType(),
        fmt::format("Shader Allocator type ({}) should be the same as Shader type ({})", m_type, alloc->ShaderType()));
}

static auto CStyle(std::span<std::string> values) {
    std::vector<GLint> lens;
    lens.reserve(values.size());
    std::vector<const char*> strs;
    strs.reserve(values.size());

    std::transform(values.begin(), values.end(), std::back_inserter(lens),
                   [](const std::string& value) { return static_cast<GLint>(value.size()); });
    std::transform(values.begin(), values.end(), std::back_inserter(strs),
                   [](const std::string& value) { return value.c_str(); });
    return std::make_tuple(strs, lens);
}

void Shader::Sources(std::span<std::string> sources) {
    m_sources.clear();
    m_sources.reserve(sources.size());
    std::copy(sources.begin(), sources.end(), std::back_inserter(m_sources));

    auto [strs, lens] = CStyle(m_sources);

    GL()->ShaderSource(RawId(), static_cast<GLsizei>(sources.size()), strs.data(), lens.data());
    debug::GLCheckError(GL());
}

bool Shader::Compile() {
    auto* gl = GL();

    gl->CompileShader(RawId());
    debug::GLCheckError(gl);

    return IsCompiled();
}

bool Shader::IsCompiled() {
    return static_cast<GLboolean>(GetParameter(GL(), Id(), Parameter::CompileStatus)) == GL_TRUE;
}

std::string Shader::InfoLog() {
    auto* gl = GL();
    GLint length = Shader::GetParameter(gl, Id(), Parameter::InfoLogLength);

    GLsizei actual_length = 0;
    std::string info_log;
    info_log.resize(length);

    gl->GetShaderInfoLog(RawId(), static_cast<GLsizei>(length), &actual_length, info_log.data());
    debug::GLCheckError(gl);

    info_log.resize(actual_length);

    return info_log;
}

VertexShader::VertexShader(Context* gl, ShaderAllocator* alloc) : Shader(GL_VERTEX_SHADER, gl, alloc) {}

FragmentShader::FragmentShader(Context* gl, ShaderAllocator* alloc) : Shader(GL_FRAGMENT_SHADER, gl, alloc) {}

}    // namespace owlet::gl
