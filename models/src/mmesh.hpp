#pragma once

#include <array>
#include <vector>

#include <glm/glm.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "shader.hpp"

namespace over {

class MVertex {
 public:
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;

  MVertex() = default;
};

class MElement {
 public:
  std::array<uint32_t, 3> indicies;

  MElement(uint32_t a, uint32_t b, uint32_t c) : indicies({a, b, c}) {}
};

class MTexture {
 public:
  enum class RType { DIFFUSE, SPECULAR, UNKNOWN };

  uint32_t id;
  RType type;
  std::string path;

  MTexture() : id(0), type(RType::UNKNOWN), path() {}
  MTexture(uint32_t id, RType type, std::string path)
      : id(id), type(type), path(path) {}

  std::string StringType() {
    switch (type) {
      case RType::DIFFUSE:
        return "diffuse";
      case RType::SPECULAR:
        return "specular";
      default:
        return "unknown";
    }
  }
};

class MMesh {
 public:
  MMesh() noexcept;
  MMesh(std::vector<MVertex> vertices, std::vector<MElement> elements,
        std::vector<MTexture> textures);

  MMesh(const MMesh&);
  MMesh(MMesh&&) noexcept;

  MMesh& operator=(const MMesh&);
  MMesh& operator=(MMesh&&) noexcept;

  ~MMesh();

  void Draw(Shader& shader);

 private:
  void Setup();

  std::vector<MVertex> vertices_;
  std::vector<MElement> elements_;
  std::vector<MTexture> textures_;

  GLuint vao_, vbo_, ibo_;
};
}  // namespace over
