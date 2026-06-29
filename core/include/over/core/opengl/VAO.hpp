#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/Binded.hpp>
#include <over/core/opengl/views/LayoutView.hpp>
#include <over/core/opengl/wrappers/LayoutWrapper.hpp>

namespace over {

class VAO : public Binded<VAO> {
 public:
  VAO() = default;

  VAO(const VAO&) = delete;
  VAO& operator=(const VAO&) = delete;

  VAO(VAO&&) noexcept = default;
  VAO& operator=(VAO&&) noexcept = default;

  ~VAO() noexcept = default;

  void AttachAttribute(uint32 location, uint32 count, GLenum type, usize size,
                       usize offset);

  void Bind() const;
  void Unbind() const noexcept;

 private:
  gl::LayoutWrapper<> _layout;
};

}  // namespace over
