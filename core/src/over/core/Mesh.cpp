#include <over/core/Mesh.hpp>

#include <array>
#include <stdexcept>
#include <string>
#include <utility>

#include <fmt/core.h>

namespace over {

Texture::Texture(uint32 id, Type type, std::string path)
    : id(id), type(type), path(std::move(path)) {}

std::string Texture::GetType() const noexcept {
  switch (type) {
    case Type::DIFFUSE:
      return "diffuse";
    case Type::SPECULAR:
      return "specular";
    default:
      return "unknown";
  }
}

Mesh::Mesh() : _vao(), _vbo(), _ibo(), _textures() {}

Mesh::Mesh(std::vector<Vertex> verticies, std::vector<Element> elements,
           std::vector<Texture> textures)
    : _vao(),
      _vbo(std::move(verticies)),
      _ibo(std::move(elements)),
      _textures(std::move(textures)) {
  Setup();
}

Mesh::Mesh(VBO vbo, IBO ibo, std::vector<Texture> textures)
    : _vao(),
      _vbo(std::move(vbo)),
      _ibo(std::move(ibo)),
      _textures(std::move(textures)) {
  Setup();
}

Mesh::Mesh(const Mesh& other) : Mesh() {
  *this = other;
}

Mesh::Mesh(Mesh&& other) noexcept : Mesh() {
  *this = std::move(other);
}

Mesh& Mesh::operator=(const Mesh& other) {
  if (this == &other) {
    return *this;
  }

  _vbo = other._vbo;
  _ibo = other._ibo;
  _textures = other._textures;
  Setup();

  return *this;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  _vao = std::move(other._vao);
  _vbo = std::move(other._vbo);
  _ibo = std::move(other._ibo);
  _textures = std::move(other._textures);

  return *this;
}

Mesh::~Mesh() {}

void Mesh::Setup() {
  _vao.Bind();
  _vbo.ToGPU();
  _ibo.ToGPU();

  _vao.AttachAttribute(0, 3, GL_FLOAT, sizeof(Vertex),
                       offsetof(Vertex, Vertex::position));
  _vao.AttachAttribute(1, 3, GL_FLOAT, sizeof(Vertex),
                       offsetof(Vertex, Vertex::normal));
  _vao.AttachAttribute(2, 2, GL_FLOAT, sizeof(Vertex),
                       offsetof(Vertex, Vertex::texCoord));

  _vao.Unbind();
  _vbo.Unbind();
  _ibo.Unbind();
}

void Mesh::Draw(Shader& shader, int32 count) {
  uint32 diffuseCounter = 0;
  uint32 specularCounter = 0;
  for (usize i = 0; i < _textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    const auto& texture = _textures[i];
    auto stype = texture.GetType();

    uint32 n = 0;
    switch (texture.type) {
      case Texture::Type::DIFFUSE:
        n = diffuseCounter++;
        break;
      case Texture::Type::SPECULAR:
        n = specularCounter++;
        break;
      default:
        throw std::runtime_error("Unknown texture type");
    }

    std::string varname = "material.texture_" + stype + std::to_string(n);
    shader.SetInt(varname, static_cast<int>(i));
    glBindTexture(GL_TEXTURE_2D, texture.id);
  }
  glActiveTexture(GL_TEXTURE0);

  _vao.Bind();

  glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(_ibo.Size()),
                          GL_UNSIGNED_INT, nullptr, count);
  _vao.Unbind();
}

}  // namespace over