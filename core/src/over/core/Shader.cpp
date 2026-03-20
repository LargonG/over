#include <over/core/Shader.hpp>

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

#include <over/core/Includes.hpp>

#include <fmt/core.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace over {

static std::string ReadShaderFile(const std::string& filename) {
  std::ifstream file(filename);
  std::stringstream shaderStream;

  if (file.fail()) {
    throw std::runtime_error(
        fmt::format("Error, while reading file: {}", filename));
  }

  shaderStream << file.rdbuf();

  return shaderStream.str();
}

static GLuint CompileShader(const char* source, GLenum type) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  int32 success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
    fmt::println("shader compilation error: {}", infoLog);
    throw fmt::system_error(-1, "shader compilation error: {}", infoLog);
  }
  return shader;
}

Shader::Shader(std::string vertexPath, std::string fragmentPath)
    : vertexPath_(std::move(vertexPath)),
      fragmentPath_(std::move(fragmentPath)),
      program_(0) {
  Compile();
}

Shader::~Shader() {
  if (program_ != 0) {
    glDeleteProgram(program_);
  }
}

void Shader::Compile() {
  std::string vertexShaderSource = "";
  std::string fragmentShaderSource = "";
  try {
    vertexShaderSource = ReadShaderFile(vertexPath_);
    fragmentShaderSource = ReadShaderFile(fragmentPath_);
  } catch (std::ifstream::failure& e) {
    fmt::println("cannot read shader file: {}", e.what());
    throw fmt::system_error(-1, "cannot read shader file: {}", e.what());
  }

  GLuint vertexShader =
      CompileShader(vertexShaderSource.c_str(), GL_VERTEX_SHADER);
  GLuint fragmentShader =
      CompileShader(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  int32 success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
    fmt::println("cannot link shader program: {}", infoLog);
    throw fmt::system_error(-1, "cannot link shader program: {}", infoLog);
  }

  if (program_ != 0) {
    glDeleteProgram(program_);
    program_ = 0;
  }
  program_ = program;
}

void Shader::Activate() noexcept {
  glUseProgram(program_);
}

GLint Shader::GetUniformLocation(const std::string& name) {
  return glGetUniformLocation(program_, name.c_str());
}

void Shader::SetBool(const std::string& name, bool value) {
  glUniform1i(GetUniformLocation(name), static_cast<int>(value));
}

void Shader::SetFloat(const std::string& name, float32 value) {
  glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetInt(const std::string& name, int32 value) {
  glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetMatrix4f(const std::string& name, float32* ptr) {
  glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, ptr);
}

void Shader::SetMatrix4f(const std::string& name, glm::mat4 mat) {
  glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE,
                     glm::value_ptr(mat));
}

void Shader::SetVec3f(const std::string& name, float32 x, float32 y,
                      float32 z) {
  glUniform3f(GetUniformLocation(name), x, y, z);
}

void Shader::SetVec3f(const std::string& name, glm::vec3 v) {
  SetVec3f(name, glm::value_ptr(v));
}

void Shader::SetVec3f(const std::string& name, float32* ptr) {
  glUniform3fv(GetUniformLocation(name), 1, ptr);
}

void Shader::SetVec3fv(const std::string& name, usize count, float32* ptr) {
  glUniform3fv(GetUniformLocation(name), count, ptr);
}

void Shader::SetVec2f(const std::string& name, float32 x, float32 y) {
  glUniform2f(GetUniformLocation(name), x, y);
}

void Shader::SetVec2f(const std::string& name, glm::vec2 v) {
  SetVec2f(name, glm::value_ptr(v));
}

void Shader::SetVec2f(const std::string& name, float32* ptr) {
  glUniform2fv(GetUniformLocation(name), 1, ptr);
}

}  // namespace over
