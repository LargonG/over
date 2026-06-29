#include <over/core/Mesh.hpp>

#include <array>
#include <stdexcept>
#include <string>
#include <utility>

#include <over/core/Shader.hpp>

#include <fmt/core.h>

namespace over {

MeshTexture::MeshTexture(View2D view, Type type) : view(view), type(type) {}

std::string MeshTexture::GetType() const noexcept {
  switch (type) {
    case Type::DIFFUSE:
      return "diffuse";
    case Type::SPECULAR:
      return "specular";
    default:
      return "unknown";
  }
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Element> elements,
           std::vector<MeshTexture> textures)
    : _vao(),
      _vbo(std::move(vertices)),
      _ibo(std::move(elements)),
      _textures(std::move(textures)) {
  Setup();
}

Mesh::Mesh(VBO vbo, IBO ibo, std::vector<MeshTexture> textures)
    : _vao(),
      _vbo(std::move(vbo)),
      _ibo(std::move(ibo)),
      _textures(std::move(textures)) {
  Setup();
}

void Mesh::Setup() {
  _vao.Use([&] {
    _vbo.ToGPU();
    _ibo.ToGPU();
    _vao.AttachAttribute(0, 3, GL_FLOAT, sizeof(Vertex),
                         offsetof(Vertex, Vertex::position));
    _vao.AttachAttribute(1, 3, GL_FLOAT, sizeof(Vertex),
                         offsetof(Vertex, Vertex::normal));
    _vao.AttachAttribute(2, 2, GL_FLOAT, sizeof(Vertex),
                         offsetof(Vertex, Vertex::texCoord));
  });

  _vbo.Unbind();
  _ibo.Unbind();
}

static void BindTextures(std::vector<MeshTexture>& textures, Shader& shader,
                         bool flush = false) {
  uint32 diffuseCounter = 0;
  uint32 specularCounter = 0;
  for (usize i = 0; i < textures.size(); i++) {
    gl::Texture::Activate(static_cast<GLenum>(GL_TEXTURE0 + i));
    const auto& texture = textures[i];
    auto stype = texture.GetType();

    uint32 n = 0;
    switch (texture.type) {
      case MeshTexture::Type::DIFFUSE:
        n = diffuseCounter++;
        break;
      case MeshTexture::Type::SPECULAR:
        n = specularCounter++;
        break;
      default:
        throw std::runtime_error("Unknown texture type");
    }

    std::string varname = "material.texture_" + stype + std::to_string(n);

    if (flush) {
      shader.SetInt(varname, 0);
      texture.view
          .Unbind();  // POTENCIAL BUG: if unbind is going to store last binded target
    } else {
      shader.SetInt(varname, static_cast<int>(i));
      texture.view.Bind();
    }
  }
  gl::Texture::Activate(GL_TEXTURE0);
}

void Mesh::Draw(Shader& shader, int32 count) {
  BindTextures(_textures, shader);

  _vao.Use([&]() {
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(_ibo.Size()),
                            GL_UNSIGNED_INT, nullptr, count);
  });

  BindTextures(_textures, shader, true);
}

void Mesh::Draw(int32 count) {
  Shader shader = Shader::GetCurrent();
  Draw(shader, count);
}

Mesh Mesh::GenQuad(std::vector<MeshTexture> textures) {
  static std::vector<Vertex> data = {
      Vertex({-1.f, -1.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f}),
      Vertex({1.f, -1.f, 0.f}, {0.f, 0.f, 0.f}, {1.f, 0.f}),
      Vertex({1.f, 1.f, 0.f}, {0.f, 0.f, 0.f}, {1.f, 1.f}),
      Vertex({-1.f, 1.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 1.f}),
  };

  static std::vector<Element> ids = {Element(0, 1, 2), Element(2, 3, 0)};

  return Mesh(data, ids, std::move(textures));
}

}  // namespace over
