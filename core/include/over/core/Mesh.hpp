#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <over/core/Includes.hpp>
#include <over/core/Shader.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/IBO.hpp>
#include <over/core/opengl/VAO.hpp>
#include <over/core/opengl/VBO.hpp>
#include <over/core/opengl/targets/TextureTarget.hpp>
#include <over/core/opengl/views/TextureView.hpp>

namespace over {

class MeshTexture {
  using View2D = gl::TextureView<gl::TextureTarget::TEXTURE_2D>;

 public:
  enum class Type { DIFFUSE, SPECULAR, UNKNOWN };

  View2D view;
  Type type;

  MeshTexture() = default;
  MeshTexture(View2D view, Type type);

  MeshTexture(const MeshTexture&) = default;
  MeshTexture& operator=(const MeshTexture&) = default;

  MeshTexture(MeshTexture&&) noexcept = default;
  MeshTexture& operator=(MeshTexture&&) noexcept = default;

  std::string GetType() const noexcept;
};

class Mesh {
 public:
  Mesh() = default;
  Mesh(std::vector<Vertex> vertices, std::vector<Element> elements,
       std::vector<MeshTexture> textures);
  Mesh(VBO vbo, IBO ibo, std::vector<MeshTexture> textures);

  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;

  Mesh(Mesh&&) noexcept = default;
  Mesh& operator=(Mesh&&) noexcept = default;

  ~Mesh() = default;

  void Draw(Shader& shader, int32 count = 1);
  void Draw(int32 count = 1);

  VBO& GetVBO() noexcept { return _vbo; }
  const VBO& GetVBO() const noexcept { return _vbo; }

  IBO& GetIBO() noexcept { return _ibo; }
  const IBO& GetIBO() const noexcept { return _ibo; }

  std::vector<MeshTexture>& GetTextures() noexcept { return _textures; }

  static Mesh GenQuad(std::vector<MeshTexture> textures);

 private:
  void Setup();

  std::vector<MeshTexture> _textures;

  VAO _vao;
  VBO _vbo;
  IBO _ibo;
};

}  // namespace over
