#pragma once

#include <functional>

namespace over {
template <typename T>
class Binded {
 public:
  template <typename F, typename... Args>
  void Use(F&& func, Args&&... args) {
    static_assert(std::is_invocable_v<F, Args...>, "F should be invokable");

    static_cast<T*>(this)->Bind();
    std::forward<F>(func)(std::forward<Args>(args)...);
    static_cast<T*>(this)->Unbind();
  }

  template <typename F, typename... Args>
  void Use(F&& func, Args&&... args) const {
    static_assert(std::is_invocable_v<F, Args...>, "F should be invokable");

    static_cast<const T*>(this)->Bind();
    std::forward<F>(func)(std::forward<Args>(args)...);
    static_cast<const T*>(this)->Unbind();
  }

  template <typename F, typename... Args>
  void Use(GLenum as, F&& func, Args&&... args) {
    static_assert(std::is_invocable_v<F, Args...>, "F should be invokable");
    // TODO: add check for bind/unbind

    static_cast<T*>(this)->Bind(as);
    std::forward<F>(func)(std::forward<Args>(args...)...);
    static_cast<T*>(this)->Unbind(as);
  }

  template <typename F, typename... Args>
  void Use(GLenum as, F&& func, Args&&... args) const {
    static_assert(std::is_invocable_v<F, Args...>, "F should be invokable");
    // TODO: add check for bind/unbind

    static_cast<const T*>(this)->Bind(as);
    std::forward<F>(func)(std::forward<Args>(args...)...);
    static_cast<const T*>(this)->Unbind(as);
  }
};
}  // namespace over
