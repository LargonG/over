#include "mesh.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <fmt/core.h>
#include <cstdint>

namespace over {
Mesh::Mesh() noexcept : vertices_(), elements_(), vao_(0), vbo_(0), ibo_(0) {}

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<Element>& elements)
    : vertices_(vertices), elements_(elements), vao_(0), vbo_(0), ibo_(0) {}

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<Element>&& elements)
    : vertices_(std::move(vertices)),
      elements_(std::move(elements)),
      vao_(0),
      vbo_(0),
      ibo_(0) {}

Mesh::Mesh(const Mesh& mesh)
    : vertices_(mesh.vertices_),
      elements_(mesh.elements_),
      vao_(0),
      vbo_(0),
      ibo_(0) {
  assert(mesh.vao_ == 0 && mesh.vbo_ == 0 && mesh.ibo_ == 0 ||
         mesh.vao_ != 0 && mesh.vbo_ != 0 && mesh.ibo_ != 0);
  if (mesh.vao_ != 0 || mesh.vbo_ != 0 || mesh.ibo_ != 0) {
    GenGPU();
  }
}

Mesh::Mesh(Mesh&& mesh) noexcept : Mesh() {
  *this = std::move(mesh);
}

Mesh& Mesh::operator=(const Mesh& mesh) {
  *this = Mesh(mesh);
  return *this;
}

Mesh& Mesh::operator=(Mesh&& mesh) noexcept {
  if (this == &mesh) {
    return *this;
  }

  vertices_ = std::exchange(mesh.vertices_, {});
  elements_ = std::exchange(mesh.elements_, {});
  vao_ = std::exchange(mesh.vao_, 0);
  vbo_ = std::exchange(mesh.vbo_, 0);
  ibo_ = std::exchange(mesh.ibo_, 0);

  return *this;
}

Mesh::~Mesh() {
  if (ibo_ != 0) {
    glDeleteBuffers(1, &ibo_);
  }
  if (vbo_ != 0) {
    glDeleteBuffers(1, &vbo_);
  }
  if (vao_ != 0) {
    glDeleteVertexArrays(1, &vao_);
  }
}

void Mesh::GenGPU() {
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &ibo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_.size() * sizeof(Element),
               elements_.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
               vertices_.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, coord)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, color)));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, texCoord)));
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::ToGPU() {
  if (vao_ == 0 || vbo_ == 0 || ibo_ == 0) {
    throw fmt::system_error(
        -1, "VAO: {} or VBO {} or IBO {} is not initialized", vao_, vbo_, ibo_);
  }

  glBindVertexArray(vao_);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_.size() * sizeof(Element),
               elements_.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
               vertices_.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Bind() {
  glBindVertexArray(vao_);
}

void Mesh::Unbind() {
  glBindVertexArray(0);
}

Mesh GenCubeMesh() {
  float rawVertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,  // 0
      0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,  // 1
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,  // 2
      -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f,  // 3

      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  // 4
      0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,  // 5
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,  // 6
      -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,  // 7

      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  // 8
      -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f,  // 9
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,  // 10
      -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f,  // 11

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  // 12
      0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  // 13
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  // 14
      0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  // 15

      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,  // 16
      0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,  // 17
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,  // 18
      -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f,  // 19

      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,  // 20
      0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,  // 21
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,  // 22
      -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,  // 23
  };

  uint32_t rawIndices[] = {
      0,  1,  2,   // 0
      2,  3,  0,   // 1
      4,  5,  6,   // 2
      6,  7,  4,   // 3
      8,  9,  10,  // 4
      10, 11, 8,   // 5
      12, 13, 14,  // 6
      14, 15, 12,  // 7
      16, 17, 18,  // 8
      18, 19, 16,  // 9
      20, 21, 22,  // 10
      22, 23, 20   // 11
  };

  const std::size_t verticesCount = 24;
  const std::size_t indicesCount = 12;

  std::vector<Vertex> vertices(verticesCount, Vertex());
  for (std::size_t i = 0; i < verticesCount; i++) {
    const int32_t sz = 8;
    vertices[i].coord = glm::vec3(rawVertices[i * sz], rawVertices[i * sz + 1],
                                  rawVertices[i * sz + 2]);
    vertices[i].normal =
        glm::vec3(rawVertices[i * sz + 3], rawVertices[i * sz + 4],
                  rawVertices[i * sz + 5]);
    vertices[i].texCoord =
        glm::vec2(rawVertices[i * sz + 6], rawVertices[i * sz + 7]);
  }
  std::vector<Element> elements(indicesCount, Element(0, 0, 0));
  for (std::size_t i = 0; i < indicesCount; i++) {
    elements[i] = Element(rawIndices[i * 3], rawIndices[i * 3 + 1],
                          rawIndices[i * 3 + 2]);
  }

  return Mesh(std::move(vertices), std::move(elements));
}

}  // namespace over