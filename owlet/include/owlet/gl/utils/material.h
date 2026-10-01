#pragma once

#include <memory>
#include <string>
#include <string_view>

#include <owlet/collections.h>
#include <owlet/debug/core.h>
#include <owlet/gl/program.h>
#include <owlet/types.h>

namespace owlet::gl {

struct Material {
  private:
    struct Uniform {
        UniformVar uniform;

        bool required = false;
    };

  public:
    Material(Program& program);

    Material& Track(const std::string& name, bool required = true);

    Material& Rebind(Program& new_program);
    Material& Refresh();

    bool ContainsUniform(const std::string& name);

    Material& UpdateUniform(const std::string& name, float x);
    Material& UpdateUniform(const std::string& name, float x, float y);
    Material& UpdateUniform(const std::string& name, float x, float y, float z);
    Material& UpdateUniform(const std::string& name, float x, float y, float z, float w);
    Material& UpdateUniform(const std::string& name, int32 count, int32 size, const float* data);

    Material& UpdateUniform(const std::string& name, int32 a);
    Material& UpdateUniform(const std::string& name, int32 a, int32 b);
    Material& UpdateUniform(const std::string& name, int32 a, int32 b, int32 c);
    Material& UpdateUniform(const std::string& name, int32 a, int32 b, int32 c, int32 d);
    Material& UpdateUniform(const std::string& name, int32 count, int32 size, const int32* data);

  private:
    Uniform UniformOrNull(const std::string& name);

    template <class... Args>
    Material& UpdateUniformT(std::string name, GLenum type, Args&&... args) {
        if (!m_program) {
            return *this;
        }

        auto uni = UniformOrNull(name);

        debug::Assert(uni.uniform.location == -1 || uni.uniform.type == type);

        m_program->UpdateUniform(uni.uniform.location, std::forward<Args>(args)...);
        return *this;
    }

    Program* m_program;
    hashmap<std::string, Uniform> m_uniforms;
};
}    // namespace owlet::gl
