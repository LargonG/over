#pragma once

#include <vector>

#include <over/core/Includes.hpp>

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

class VBO {
 public:
  VBO();
  explicit VBO(std::vector<Vertex> vertices, GLenum usage = GL_STATIC_DRAW);

  VBO(const VBO&);
  VBO(VBO&&) noexcept;

  VBO& operator=(const VBO&);
  VBO& operator=(VBO&&) noexcept;

  ~VBO() noexcept;

  void ToGPU(bool unbind = false);
  void FreeGPU() const noexcept;

  void Bind(bool copy = false) const;
  void Unbind() const;

  std::vector<Vertex>& GetVerticies() noexcept { return _verticies; }
  const std::vector<Vertex>& GetVerticies() const noexcept { return _verticies; }

 private:
  std::vector<Vertex> _verticies;

  mutable GLuint _id;
  mutable GLenum _usage;
};

}  // namespace over
