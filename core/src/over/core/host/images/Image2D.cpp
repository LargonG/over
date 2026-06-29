#include <over/core/host/images/Image2D.hpp>

#include <fmt/core.h>
#include <stb_image.h>

namespace over::host {

namespace {

class SbtiImageWrapper final {
 public:
  SbtiImageWrapper(std::string_view filename)
      : width(0), height(0), channels(0), data(nullptr) {
    Load(filename);
  }

  ~SbtiImageWrapper() { Free(); }

  void Load(std::string_view filename) {
    data = stbi_load(filename.data(), &width, &height, &channels, 0);
  }

  void Free() noexcept {
    if (data == nullptr) {
      return;
    }

    stbi_image_free(data);
    data = nullptr;
  }

  int32 width, height, channels;
  void* data;
};
}  // namespace

Image2D::Image2D() : _width(0), _height(0), _channels(0), _data() {}

Image2D::Image2D(usize width, usize height, usize channels, const void* data)
    : _width(width),
      _height(height),
      _channels(channels),
      _data(width * height * channels) {
  if (data != nullptr) {
    const auto* bytes = static_cast<const std::byte*>(data);

    std::copy(bytes, bytes + _data.size(), _data.begin());
  }
}

Image2D Image2D::FromFile(std::string_view filename) {
  SbtiImageWrapper sbti(filename);
  return Image2D(sbti.width, sbti.height, sbti.channels, sbti.data);
}

}  // namespace over::host
