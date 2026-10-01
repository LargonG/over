#include <owlet/gl/utils/material.h>

#include <string>
#include <string_view>

#include <owlet/debug/gl.h>
#include <owlet/debug/log.h>
#include <owlet/gl/core.h>
#include <owlet/gl/program.h>
#include <owlet/types.h>

#include <fmt/format.h>

namespace owlet::gl {
Material::Material(Program& program) : m_program(), m_uniforms() {
    Rebind(program);
}

Material& Material::Track(const std::string& name, bool required) {
    std::string sname = std::string(name);
    debug::Require(!(required && !m_uniforms.contains(name)), "Name is required, but not found");

    m_uniforms.at(sname).required = required;

    return *this;
}

Material& Material::Rebind(Program& new_program) {
    m_program = &new_program;

    return Refresh();
}

Material& Material::Refresh() {
    auto uniforms = m_program->ActiveUniforms();

    // copy!
    auto previous = m_uniforms;
    m_uniforms.clear();

    for (usize i = 0; i < uniforms.size(); i++) {
        bool required = previous.contains(uniforms[i].name) && previous.at(uniforms[i].name).required;
        m_uniforms[uniforms[i].name] = {
            .uniform = uniforms[i],
            .required = required,
        };
        previous.erase(uniforms[i].name);
    }

    for (const auto& [key, value] : previous) {
        debug::Assert(!value.required, "Required uniform not found in new program");
    }

    return *this;
}

bool Material::ContainsUniform(const std::string& name) {
    return m_uniforms.contains(name);
}

Material& Material::UpdateUniform(const std::string& name, float x) {
    return UpdateUniformT(name, GL_FLOAT, x);
}

Material& Material::UpdateUniform(const std::string& name, float x, float y) {
    return UpdateUniformT(name, GL_FLOAT_VEC2, x, y);
}

Material& Material::UpdateUniform(const std::string& name, float x, float y, float z) {
    return UpdateUniformT(name, GL_FLOAT_VEC3, x, y, z);
}

Material& Material::UpdateUniform(const std::string& name, float x, float y, float z, float w) {
    return UpdateUniformT(name, GL_FLOAT_VEC4, x, y, z, w);
}

Material& Material::UpdateUniform(const std::string& name, int32 count, int32 size, const float* data) {
    switch (size) {
        case 1:
            return UpdateUniformT(name, GL_FLOAT, count, size, data);
        case 2:
            return UpdateUniformT(name, GL_FLOAT_VEC2, count, size, data);
        case 3:
            return UpdateUniformT(name, GL_FLOAT_VEC3, count, size, data);
        case 4:
            return UpdateUniformT(name, GL_FLOAT_VEC4, count, size, data);
    }
    debug::Unreachable(fmt::format("Unexpected size: {}", size));
}

Material& Material::UpdateUniform(const std::string& name, int32 a) {
    return UpdateUniformT(name, GL_INT, a);
}

Material& Material::UpdateUniform(const std::string& name, int32 a, int32 b) {
    return UpdateUniformT(name, GL_INT_VEC2, a, b);
}

Material& Material::UpdateUniform(const std::string& name, int32 a, int32 b, int32 c) {
    return UpdateUniformT(name, GL_INT_VEC3, a, b, c);
}

Material& Material::UpdateUniform(const std::string& name, int32 a, int32 b, int32 c, int32 d) {
    return UpdateUniformT(name, GL_INT_VEC4, a, b, c, d);
}

Material& Material::UpdateUniform(const std::string& name, int32 count, int32 size, const int32* data) {
    switch (size) {
        case 1:
            return UpdateUniformT(name, GL_INT, count, size, data);
        case 2:
            return UpdateUniformT(name, GL_INT_VEC2, count, size, data);
        case 3:
            return UpdateUniformT(name, GL_INT_VEC3, count, size, data);
        case 4:
            return UpdateUniformT(name, GL_INT_VEC4, count, size, data);
    }
    debug::Unreachable(fmt::format("Unexpected size: {}", size));
}

owlet::gl::Material::Uniform Material::UniformOrNull(const std::string& name) {
    return m_uniforms.contains(name) ? m_uniforms.at(name)
                                     : Uniform{
                                           .uniform =
                                               {
                                                   .name = name,
                                               },
                                       };
}

}    // namespace owlet::gl
