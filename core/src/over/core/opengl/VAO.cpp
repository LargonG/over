#include <over/core/opengl/VAO.hpp>

#include <cassert>
#include <utility>

#include <over/core/Includes.hpp>

namespace over {

VAO::VAO() : _id(0), _isActive(false) {}

VAO::VAO(VAO&& other) noexcept : VAO() {
  *this = std::move(other);
}

VAO& VAO::operator=(VAO&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  _id = std::exchange(other._id, 0);
  _isActive = std::exchange(other._isActive, false);

  return *this;
}

VAO::~VAO() noexcept {
  FreeGPU();
}

void VAO::AttachAttribute(uint32 location, uint32 count, GLenum type,
                          usize size, usize offset) {
  assert(_isActive);
  glVertexAttribPointer(location, count, type, GL_FALSE, size,
                        reinterpret_cast<void*>(offset));
  glEnableVertexAttribArray(location);
}

void VAO::Bind() const {
  _isActive = true;
  if (_id == 0) {
    glGenVertexArrays(1, &_id);
  }

  glBindVertexArray(_id);
}

void VAO::Unbind() const noexcept {
  _isActive = false;
  glBindVertexArray(0);
}

void VAO::FreeGPU() const noexcept {
  if (_id == 0) {
    return;
  }
  Unbind();
  glDeleteVertexArrays(1, &_id);
  _id = 0;
  _isActive = false;
}
}  // namespace over
