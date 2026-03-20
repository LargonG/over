#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>

#include <glm/glm.hpp>
#include <string>

namespace over {
class Shader {
 public:
  Shader(std::string vertexPath, std::string fragmentPath);

  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  Shader(Shader&&) = delete;
  Shader& operator=(Shader&&) = delete;

  ~Shader();

  void Activate() noexcept;

  GLuint GetProgram() const noexcept { return program_; };
  GLint GetUniformLocation(const std::string& name);

  void SetBool(const std::string& name, bool value);
  void SetFloat(const std::string& name, float32 value);
  void SetInt(const std::string& name, int32 value);

  void SetMatrix4f(const std::string& name, float32* ptr);
  void SetMatrix4f(const std::string& name, glm::mat4 mat);

  void SetVec3f(const std::string& name, float32 x, float32 y, float32 z);
  void SetVec3f(const std::string& name, glm::vec3 v);
  void SetVec3f(const std::string& name, float32* ptr);
  void SetVec3fv(const std::string& name, usize count, float32* ptr);

  void SetVec2f(const std::string& name, float32 x, float32 y);
  void SetVec2f(const std::string& name, glm::vec2 v);
  void SetVec2f(const std::string& name, float32* ptr);

 private:
  void Compile();

  std::string vertexPath_;
  std::string fragmentPath_;

  GLuint program_;
};
}  // namespace over
