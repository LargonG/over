#pragma once

#include <fmt/core.h>
#include <exception>

#include <over/core/Includes.hpp>

#define glthrow(expr) \
  expr;               \
  over::gl::Exception::Throw();

namespace over::gl {
class Exception : public std::exception {
 public:
  Exception(GLenum code) : std::exception("OpenGL exception") {
    _value = fmt::format("OpenGL exception, code: {}", code);
  }

  template <class F, class... Args>
  static void Try(F&& func, Args&&... args) {
    std::forward<F>(func)(std::forward<Args>(args)...);
    Throw();
  }

  static void Throw() {
    auto res = glGetError();
    if (0 != res) {
      throw gl::Exception(res);
    }
  }

  virtual char const* what() const override { return _value.c_str(); }

 private:
  std::string _value;
};
}  // namespace over::gl
