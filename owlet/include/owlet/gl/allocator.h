#pragma once

#include <glad/gl.h>

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

#define WL_DEF_GL_ID(name) enum class name##Id : GLuint{Null = 0};
#define WL_DEF_GL_ALLOC(name) \
    struct name##Allocator : Allocator<name##Id> {};

#define WL_DEF_ALLOC_IMPL(name) WL_DEF_GL_ID(name) WL_DEF_GL_ALLOC(name)

WL_DEF_ALLOC_IMPL(Buffer);
WL_DEF_ALLOC_IMPL(Texture);
WL_DEF_ALLOC_IMPL(RenderBuffer);
WL_DEF_ALLOC_IMPL(FrameBuffer);
WL_DEF_ALLOC_IMPL(VertexArray);

}    // namespace owlet::gl
