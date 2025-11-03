#pragma once

#include <array>
#include <cstdint>
#include <glm/glm.hpp>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace over {
struct Vertex {
 public:
  glm::vec3 coord;
  glm::vec3 normal;
  glm::vec3 color;
  glm::vec2 texCoord;

  Vertex() : coord(), normal(), color(), texCoord() {}

  Vertex(float x, float y, float z, float nx, float ny, float nz, float r,
         float g, float b, float s, float t) noexcept
      : coord(x, y, z), normal(nx, ny, nz), color(r, g, b), texCoord(s, t) {}

  Vertex(glm::vec3 coord, glm::vec3 normal, glm::vec3 color,
         glm::vec2 texCoord) noexcept
      : coord(std::move(coord)),
        normal(std::move(normal)),
        color(std::move(color)),
        texCoord(std::move(texCoord)) {}
};

struct Element {
 public:
  std::array<uint32_t, 3> indices;

  explicit Element(std::array<uint32_t, 3> indices)
      : indices(std::move(indices)) {}
  Element(uint32_t a, uint32_t b, uint32_t c) : indices({a, b, c}) {}
};

class Mesh {
 public:
  Mesh() noexcept;
  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<Element>& elements);
  Mesh(std::vector<Vertex>&& vertices, std::vector<Element>&& elements);

  Mesh(const Mesh&);
  Mesh(Mesh&&) noexcept;

  Mesh& operator=(const Mesh&);
  Mesh& operator=(Mesh&&) noexcept;

  ~Mesh();

  void GenGPU();
  void ToGPU();
  void Bind();
  void Unbind();

  const std::vector<Vertex>& Vertices() const noexcept { return vertices_; }
  const std::vector<Element>& Elements() const noexcept { return elements_; }

  std::vector<Vertex>& Vertices() noexcept { return vertices_; }
  std::vector<Element>& Elements() noexcept { return elements_; }

  GLuint VAO() const noexcept { return vao_; }

 private:
  std::vector<Vertex> vertices_;
  std::vector<Element> elements_;

  GLuint vao_;
  GLuint vbo_;
  GLuint ibo_;
};

Mesh GenCubeMesh();
}  // namespace over