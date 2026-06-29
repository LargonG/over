#pragma once

#include <vector>

#include <over/core/Includes.hpp>
#include <over/core/opengl/Binded.hpp>
#include <over/core/opengl/views/BufferView.hpp>
#include <over/core/opengl/wrappers/BufferWrapper.hpp>

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
  VBO() = default;
  explicit VBO(std::vector<Vertex> vertices, GLenum usage = GL_STATIC_DRAW);

  VBO(const VBO&) = delete;
  VBO& operator=(const VBO&) = delete;

  VBO(VBO&&) noexcept = default;
  VBO& operator=(VBO&&) noexcept = default;

  ~VBO() = default;

  void ToGPU(bool unbind = false);

  void Bind(bool copy = false) const;
  void Unbind() const;

  std::vector<Vertex>& GetVerticies() noexcept { return _vertices; }
  const std::vector<Vertex>& GetVerticies() const noexcept { return _vertices; }

 private:
  std::vector<Vertex> _vertices;

  gl::BufferWrapper<> _buffer;
  GLenum _usage;
};

}  // namespace over
