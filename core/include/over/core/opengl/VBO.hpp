#pragma once

#include <vector>

#include <over/core/Includes.hpp>
#include <over/core/opengl/Binded.hpp>
#include <over/core/opengl/buffers/Buffer.hpp>

#include <glm/glm.hpp>

namespace over {

class Vertex {
 public:
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;

  Vertex() = default;
  Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord);
};

class VBO : public Binded<VBO> {
 public:
  VBO();
  explicit VBO(std::vector<Vertex> vertices, GLenum usage = GL_STATIC_DRAW);

  VBO(const VBO&) = default;
  VBO(VBO&&) noexcept = default;

  VBO& operator=(const VBO&) = default;
  VBO& operator=(VBO&&) noexcept = default;

  ~VBO() noexcept;

  void ToGPU(bool unbind = false);
  void FreeGPU() const noexcept;

  void Bind(bool copy = false) const;
  void Unbind() const;

  std::vector<Vertex>& GetVerticies() noexcept { return _verticies; }
  const std::vector<Vertex>& GetVerticies() const noexcept {
    return _verticies;
  }

  Buffer<GL_ARRAY_BUFFER>& GetBuffer() const noexcept { return _buffer; }

 private:
  std::vector<Vertex> _verticies;

  mutable Buffer<GL_ARRAY_BUFFER> _buffer;
  mutable GLenum _usage;
};

}  // namespace over
