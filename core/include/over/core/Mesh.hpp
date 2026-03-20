#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <over/core/Includes.hpp>
#include <over/core/Shader.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/IBO.hpp>
#include <over/core/opengl/VAO.hpp>
#include <over/core/opengl/VBO.hpp>

namespace over {

class Texture {
 public:
  enum class Type { DIFFUSE, SPECULAR, UNKNOWN };

  uint32 id;
  Type type;
  std::string path;

  Texture() = default;
  Texture(uint32 id, Type type, std::string path);

  std::string GetType() const noexcept;
};

class Mesh {
 public:
  Mesh(std::vector<Vertex> verticies, std::vector<Element> elements,
       std::vector<Texture> textures);
  Mesh(VBO vbo, IBO ibo, std::vector<Texture> textures);

  Mesh(const Mesh&);
  Mesh(Mesh&&) noexcept;

  Mesh& operator=(const Mesh&);
  Mesh& operator=(Mesh&&) noexcept;

  ~Mesh();

  void Draw(Shader& shader, int32 count = 1);

  VBO& GetVBO() noexcept { return _vbo; }
  const VBO& GetVBO() const noexcept { return _vbo; }

  IBO& GetIBO() noexcept { return _ibo; }
  const IBO& GetIBO() const noexcept { return _ibo; }

 private:
  Mesh();

  void Setup();

  std::vector<Texture> _textures;

  VAO _vao;
  VBO _vbo;
  IBO _ibo;
};

}  // namespace over
