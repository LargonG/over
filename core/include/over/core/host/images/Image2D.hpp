#pragma once

#include <string_view>
#include <vector>

#include <over/core/Types.hpp>

namespace over::host {
class Image2D {
 public:
  Image2D();
  Image2D(usize width, usize height, usize channels, const void* data);

  Image2D(const Image2D&) = default;
  Image2D& operator=(const Image2D&) = default;

  Image2D(Image2D&&) noexcept = default;
  Image2D& operator=(Image2D&&) noexcept = default;

  ~Image2D() = default;

  usize Width() const noexcept { return _width; }
  usize Height() const noexcept { return _height; }
  usize Channels() const noexcept { return _channels; }
  const std::vector<std::byte>& Data() const noexcept { return _data; }

  static Image2D FromFile(std::string_view);

 private:
  usize _width, _height;
  usize _channels;

  std::vector<std::byte> _data;
};
}  // namespace over::host
