#pragma once

#include <functional>

namespace over {
template <typename T>
class Binded {
 public:
  Binded() {
    static_assert(std::is_base_of_v<Binded<T>, T>,
                  "T should be inherit from Binded<T>");
  }

  template <typename F>
  std::enable_if_t<std::is_invocable_v<F> && !std::is_invocable_v<F, T*>> Use(
      F&& func) {
    static_cast<T*>(this)->Bind();
    std::forward<F>(func)();
    static_cast<T*>(this)->Unbind();
  }

  template <typename F>
  std::enable_if_t<std::is_invocable_v<F> && !std::is_invocable_v<F, T*>> Use(
      F&& func) const {
    static_cast<const T*>(this)->Bind();
    std::forward<F>(func)();
    static_cast<const T*>(this)->Unbind();
  }

  template <typename F>
  std::enable_if_t<std::is_invocable_v<F, T&>> Use(F&& func) {
    static_cast<T*>(this)->Bind();
    std::forward<F>(func)(static_cast<T&>(*this));
    static_cast<T*>(this)->Unbind();
  }

  template <typename F>
  std::enable_if_t<std::is_invocable_v<F, const T&>> Use(F&& func) const {
    static_cast<const T*>(this)->Bind();
    std::forward<F>(func)(static_cast<const T&>(*this));
    static_cast<const T*>(this)->Unbind();
  }
};

}  // namespace over
