#pragma once

#include <functional>

namespace over {
template <typename T>
class Binded {
 public:
  template <typename F>
  void Use(F&& func) {
    static_cast<T*>(this)->Bind();
    std::forward<F>(func)();
    static_cast<T*>(this)->Unbind();
  }
};
}  // namespace over
