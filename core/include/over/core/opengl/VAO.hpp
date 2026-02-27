#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>

namespace over {

class VAO {
 public:
  VAO();

  VAO(const VAO&) = delete;
  VAO(VAO&&) noexcept;

  VAO& operator=(const VAO&) = delete;
  VAO& operator=(VAO&&) noexcept;

  ~VAO() noexcept;

  void AttachAttribute(uint32 location, uint32 count, GLenum type, usize size,
                       usize offset);

  void Bind() const;
  void Unbind() const noexcept;

  void FreeGPU() const noexcept;

 private:
  mutable GLuint _id;
  mutable bool _isActive;
};

}  // namespace over
