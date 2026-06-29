#include <over/core/opengl/VBO.hpp>

#include <cassert>
#include <utility>

#include <over/core/Includes.hpp>

namespace over {
Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord)
    : position(position), normal(normal), texCoord(texCoord) {}

VBO::VBO(std::vector<Vertex> vertices, GLenum usage)
    : _buffer(), _vertices(std::move(vertices)), _usage(usage) {}

void VBO::ToGPU(bool unbind) {
  Bind(true);

  if (unbind) {
    Unbind();
  }
}

void VBO::Bind(bool copy) const {
  auto view = _buffer.As<gl::BufferTarget::ARRAY_BUFFER>();
  view.Bind();

  if (copy) {
    // legacy
    const_cast<gl::BufferView<gl::BufferTarget::ARRAY_BUFFER>&>(view).Reserve(
        sizeof(Vertex) * _vertices.size(),
        reinterpret_cast<const ubyte*>(_vertices.data()), _usage);
  }
}

void VBO::Unbind() const {
  _buffer.As<gl::BufferTarget::ARRAY_BUFFER>().Unbind();
}
}  // namespace over
