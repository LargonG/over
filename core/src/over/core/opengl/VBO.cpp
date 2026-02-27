#include <over/core/opengl/VBO.hpp>

#include <cassert>
#include <utility>

#include <over/core/Includes.hpp>

namespace over {
Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord)
    : position(position), normal(normal), texCoord(texCoord) {}

VBO::VBO() : _id(0), _verticies(), _usage(GL_STATIC_DRAW) {}

VBO::VBO(std::vector<Vertex> verticies, GLenum usage)
    : _id(0), _verticies(std::move(verticies)), _usage(usage) {}

VBO::VBO(const VBO& other) : VBO() {
  *this = other;
}

VBO::VBO(VBO&& other) noexcept : VBO() {
  *this = std::move(other);
}

VBO& VBO::operator=(const VBO& other) {
  if (this == &other) {
    return *this;
  }

  // TODO: add copy on write
  _verticies = other._verticies;
  _usage = other._usage;

  return *this;
}

VBO& VBO::operator=(VBO&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  _id = std::exchange(other._id, 0);
  _verticies = std::move(other._verticies);
  _usage = std::exchange(other._usage, GL_STATIC_DRAW);

  return *this;
}

VBO::~VBO() noexcept {
  FreeGPU();
}

void VBO::ToGPU(bool unbind) {
  //assert(_verticies.size() > 0);

  // TODO: add method to borrow id from other ids allocator
  if (_id == 0) {
    glGenBuffers(1, &_id);
  }

  Bind(true);

  if (unbind) {
    Unbind();
  }
}

void VBO::FreeGPU() const noexcept {
  if (_id == 0) {
    return;
  }

  glDeleteBuffers(1, &_id);
  _id = 0;
}

void VBO::Bind(bool copy) const {
  assert(_id != 0);
  glBindBuffer(GL_ARRAY_BUFFER, _id);
  if (copy) {
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _verticies.size(),
                 _verticies.data(), _usage);
  }
}

void VBO::Unbind() const {
  assert(_id != 0);
  glBindBuffer(GL_ARRAY_BUFFER, _id);
}
}  // namespace over
