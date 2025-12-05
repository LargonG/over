#include "mmesh.hpp"

#include <fmt/core.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace over {
MMesh::MMesh() noexcept
    : vertices_(), elements_(), textures_(), vao_(0), vbo_(0), ibo_(0) {}

MMesh::MMesh(std::vector<MVertex> vertices, std::vector<MElement> elements,
             std::vector<MTexture> textures)
    : vertices_(std::move(vertices)),
      elements_(std::move(elements)),
      textures_(std::move(textures)),
      vao_(0),
      vbo_(0),
      ibo_(0) {
  Setup();
}

MMesh::MMesh(const MMesh& other)
    : MMesh(other.vertices_, other.elements_, other.textures_) {}

MMesh::MMesh(MMesh&& other) noexcept : MMesh() {
  *this = std::move(other);
}

MMesh& MMesh::operator=(const MMesh& other) {
  if (this == &other) {
    return *this;
  }

  *this = MMesh(other);

  return *this;
}

MMesh& MMesh::operator=(MMesh&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  vertices_ = std::move(other.vertices_);
  elements_ = std::move(other.elements_);
  textures_ = std::move(other.textures_);
  vao_ = std::exchange(other.vao_, 0);
  vbo_ = std::exchange(other.vbo_, 0);
  ibo_ = std::exchange(other.ibo_, 0);

  return *this;
}

MMesh::~MMesh() {
  if (vao_ == 0) {
    return;
  }

  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
  glDeleteBuffers(1, &ibo_);
}

void MMesh::Setup() {
  if (vao_ != 0) {
    return;
  }

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ibo_);

  glBindVertexArray(vao_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(MVertex) * vertices_.size(),
               vertices_.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MElement) * elements_.size(),
               elements_.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE, sizeof(MVertex),
      reinterpret_cast<void*>(offsetof(MVertex, MVertex::position)));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE, sizeof(MVertex),
      reinterpret_cast<void*>(offsetof(MVertex, MVertex::normal)));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(
      2, 2, GL_FLOAT, GL_FALSE, sizeof(MVertex),
      reinterpret_cast<void*>(offsetof(MVertex, MVertex::texCoord)));

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void over::MMesh::Draw(Shader& shader) {
  uint32_t diffuseCounter = 0;
  uint32_t specularCounter = 0;
  for (size_t i = 0; i < textures_.size(); i++) {
    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + i));
    auto& texture = textures_[i];
    uint32_t n = 0;
    std::string name;

    switch (texture.type) {
      case MTexture::RType::DIFFUSE:
        n = diffuseCounter++;
        name = "diffuse";
        break;
      case MTexture::RType::SPECULAR:
        n = specularCounter++;
        name = "specular";
        break;
      default:
        fmt::println("Unknown texture type: {}", texture.StringType());
        throw fmt::system_error(-1, "Unknown texture type: {}",
                                texture.StringType());
    }

    std::string letter = "material.texture_" + name + std::to_string(n);
    shader.SetInt(letter, static_cast<int>(i));
    glBindTexture(GL_TEXTURE_2D, texture.id);
  }
  glActiveTexture(GL_TEXTURE0);

  assert(vao_ != 0 && vbo_ != 0 && ibo_ != 0);

  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(3 * elements_.size()),
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

}  // namespace over
