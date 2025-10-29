#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <string>

namespace over {
class Shader {
 public:
  Shader(std::string vertexPath, std::string fragmentPath);

  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  Shader(Shader&&) = delete;
  Shader& operator=(Shader&&) = delete;

  void Compile();
  void Activate() noexcept;

  GLuint GetProgram() const noexcept { return program_; };
  GLint GetUniformLocation(const std::string& name);

  void SetBool(const std::string& name, bool value);
  void SetFloat(const std::string& name, float value);
  void SetInt(const std::string& name, int value);
  void SetMatrix4f(const std::string& name, float* ptr);

 private:
  std::string vertexPath_;
  std::string fragmentPath_;

  GLuint program_;
};
}  // namespace over