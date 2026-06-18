#include <over/core/opengl/VBO.hpp>

#include <cassert>
#include <utility>

#include <over/core/Includes.hpp>

namespace over {
Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord)
    : position(position), normal(normal), texCoord(texCoord) {}

VBO::VBO() : _buffer(), _verticies(), _usage(GL_STATIC_DRAW) {}

VBO::VBO(std::vector<Vertex> verticies, GLenum usage)
    : _buffer(), _verticies(std::move(verticies)), _usage(usage) {}

VBO::~VBO() noexcept {
  FreeGPU();
}

void VBO::ToGPU(bool unbind) {
  _buffer.Alloc();

  Bind(true);

  if (unbind) {
    Unbind();
  }
}

void VBO::FreeGPU() const noexcept {
  _buffer.Free();
}

void VBO::Bind(bool copy) const {
  _buffer.Bind();

  if (copy) {
    _buffer.Reserve(sizeof(Vertex) * _verticies.size(),
                    reinterpret_cast<const ubyte*>(_verticies.data()), _usage);
  }
}

void VBO::Unbind() const {
  _buffer.Unbind();
}
}  // namespace over
