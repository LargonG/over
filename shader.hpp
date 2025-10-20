#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <string>

namespace over {
class Shader {
 public:
  Shader(const std::string& vertexPath, const std::string& fragmentPath);

  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  Shader(Shader&&) = delete;
  Shader& operator=(Shader&&) = delete;

  void Compile();
  void Activate() noexcept;

  GLuint GetProgram() const noexcept { return program_; };

  void SetBool(const std::string& name, bool value);
  void SetFloat(const std::string& name, float value);
  void SetInt(const std::string& name, int value);

 private:
  std::string vertexPath_;
  std::string fragmentPath_;

  GLuint program_;
};
}  // namespace over