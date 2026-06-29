#pragma once

#include <stb_image.h>

#include <over/core/host/images/Image2D.hpp>
#include <over/core/opengl/Address.hpp>
#include <over/core/opengl/Texture.hpp>
#include <over/core/opengl/allocators/DefaultTextureAllocator.hpp>
#include <over/core/opengl/targets/TextureTarget.hpp>
#include <over/core/opengl/wrappers/Wrapper.hpp>

namespace over::gl {

template <TextureTarget Target>
class TextureView;

template <class Allocator = DefaultTextureAllocator>
class TextureWrapper final
    : public Wrapper<Allocator, TextureTarget, TextureWrapper<Allocator>,
                     TextureView> {
 public:
  TextureWrapper() = default;

  template <TextureTarget Target>
  TextureWrapper(const host::Image2D& img) : TextureWrapper() {
    *this = std::move(FromImage2D<Target>(img));
  }

  template <TextureTarget Target>
  static TextureWrapper<> FromImage2D(const host::Image2D& img) {
    TextureWrapper res;
    res.As<Target>([&](gl::TextureView<Target>& self) {
      GLenum format = Texture::GetFormat(img.Channels());
      self.Reserve2D(format, static_cast<usize>(img.Width()),
                     static_cast<usize>(img.Height()), format, GL_UNSIGNED_BYTE,
                     img.Data().data());
    });

    return res;
  }
};
}  // namespace over::gl
