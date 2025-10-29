#include "shader.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <string>

#include <fmt/core.h>

namespace over {

static std::string ReadShaderFile(const std::string& filename) {
  std::ifstream file(filename);
  std::stringstream shaderStream;

  shaderStream << file.rdbuf();

  return shaderStream.str();
}

static GLuint CompileShader(const char* source, GLenum type) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  int32_t success;
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
      program_(0) {}

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

  int32_t success;
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
  glUniform1i(glGetUniformLocation(program_, name.c_str()),
              static_cast<int>(value));
}

void Shader::SetFloat(const std::string& name, float value) {
  glUniform1f(glGetUniformLocation(program_, name.c_str()), value);
}

void Shader::SetInt(const std::string& name, int value) {
  glUniform1i(glGetUniformLocation(program_, name.c_str()), value);
}

void Shader::SetMatrix4f(const std::string& name, float* ptr) {
  glUniformMatrix4fv(this->GetUniformLocation(name), 1, GL_FALSE, ptr);
}

}  // namespace over