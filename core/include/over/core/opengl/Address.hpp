#pragma once

#include <cassert>
#include <type_traits>

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>

namespace over::gl {
class Address final {
 public:
  Address(GLuint id) noexcept : _id(id) {}
  Address(nullptr_t) noexcept : _id(0) {}

  Address(const Address&) = default;
  Address& operator=(const Address&) = default;

  Address(Address&&) noexcept = default;
  Address& operator=(Address&&) noexcept = default;

  ~Address() = default;

  [[nodiscard]] GLuint& operator*() noexcept { return _id; }
  [[nodiscard]] GLuint operator*() const noexcept { return _id; }

 private:
  GLuint _id;
};

}  // namespace over::gl
