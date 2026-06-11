#pragma once

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/Binded.hpp>

namespace over {

class Texture;

class Texture2D : public Binded<Texture2D> {
 public:
  Texture2D() noexcept;
  Texture2D(uint32 width, uint32 height, const ubyte* data, GLenum format);

  Texture2D(const Texture2D&);
  Texture2D(Texture2D&&) noexcept;

  Texture2D& operator=(const Texture2D&);
  Texture2D& operator=(Texture2D&&) noexcept;

  explicit operator Texture() const;

  ~Texture2D();

  void ToGPU(const ubyte* data, GLenum format, bool unbind = false);
  void FreeGPU() noexcept;

  uint32 Get() const noexcept { return _id; }

  void Bind() const noexcept;
  void Unbind() const noexcept;

 private:
  void Setup(const ubyte* data, GLenum format);

  GLenum _format;

  uint32 _width;
  uint32 _height;
  uint32 _id;
};
}  // namespace over
