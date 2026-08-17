#pragma once

namespace owlet::gl {

struct Context;

template <class Id>
struct Allocator {
  public:
    Allocator() noexcept {}

    virtual ~Allocator() {}

    virtual [[nodiscard]] Id Alloc(Context*) = 0;
    virtual void Dealloc(Context*, Id) noexcept = 0;
};

}    // namespace owlet::gl
