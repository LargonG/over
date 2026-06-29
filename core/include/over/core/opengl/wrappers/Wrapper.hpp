#pragma once

#include <type_traits>

#include <over/core/opengl/Address.hpp>

namespace over::gl {
template <class Allocator, typename TargetList, class Owner,
          template <auto> class View>
class Wrapper {
  using ConstOwner = std::add_const_t<Owner>;
  using MutableOwner = std::remove_const_t<Owner>;

 public:
  Wrapper() : _ptr(nullptr) {
    static_assert(
        std::is_base_of_v<Wrapper<Allocator, TargetList, Owner, View>, Owner>,
        "Problem");
  }

  // Note: Wrapper is the same as *ptr = address, raw int in CPU (GPU in this case) memory
  Wrapper(const Wrapper&) = delete;
  Wrapper& operator=(const Wrapper&) = delete;

  Wrapper(Wrapper&& other) noexcept : Wrapper() { *this = std::move(other); }

  Wrapper& operator=(Wrapper&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    Allocator().Dealloc(_ptr);

    *_ptr = std::exchange(*other._ptr, 0);

    return *this;
  }

  ~Wrapper() { Allocator().Dealloc(_ptr); }

  template <TargetList Target>
  View<Target> As() {
    if (0 == *_ptr) {
      _ptr = Allocator().Alloc();
    }
    return View<Target>(static_cast<Owner*>(this));
  }

  template <TargetList Target>
  const View<Target> As() const {
    return const_cast<MutableOwner*>(static_cast<ConstOwner*>(this))
        ->As<Target>();
  }

  template <TargetList Target, class F>
  void As(F&& func) {
    As<Target>().Use(std::forward<F>(func));
  }

  template <TargetList Target, class F>
  void As(F&& func) const {
    const_cast<MutableOwner*>(static_cast<ConstOwner*>(this))
        ->As<Target>(std::forward<F>(func));
  }

  const Address& Get() const noexcept { return _ptr; }

 private:
  Address _ptr;
};
}  // namespace over::gl
