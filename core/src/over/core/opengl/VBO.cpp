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
  //assert(_verticies.size() > 0);

  // TODO: add method to borrow id from other ids allocator
  /*if (_id == 0) {
    glGenBuffers(1, &_id);
  }*/

  _buffer.Alloc();

  Bind(true);

  if (unbind) {
    Unbind();
  }
}

void VBO::FreeGPU() const noexcept {
  /*if (_id == 0) {
    return;
  }

  glDeleteBuffers(1, &_id);
  _id = 0;*/

  _buffer.Free();
}

void VBO::Bind(bool copy) const {
  //assert(_id != 0);
  //  glBindBuffer(GL_ARRAY_BUFFER, _id);
  //  if (copy) {
  //    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _verticies.size(),
  //                 _verticies.data(), _usage);
  //  }

  _buffer.Bind();

  if (copy) {
    _buffer.Reserve(sizeof(Vertex) * _verticies.size(),
                    reinterpret_cast<const over::ubyte*>(_verticies.data()),
                    _usage);
  }
}

void VBO::Unbind() const {
  //assert(_id != 0);
  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  _buffer.Unbind();
}
}  // namespace over
