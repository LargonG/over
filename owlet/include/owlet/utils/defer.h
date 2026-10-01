#pragma once

#include <memory>

namespace owlet {
template <typename F>
struct Defer {
  public:
    Defer(F&& value) : m_value(std::forward<F>(value)) {}

    ~Defer() { m_value(); }

  private:
    F&& m_value;
};
}    // namespace owlet
