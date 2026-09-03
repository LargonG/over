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

#define AllocatorImpl(name, id)          \
    enum class id : GLuint { Null = 0 }; \
    struct name : Allocator<id> {}

AllocatorImpl(BufferAllocator, BufferId);
AllocatorImpl(TextureAllocator, TextureId);
AllocatorImpl(RenderBufferAllocator, RenderBufferId);
AllocatorImpl(FrameBufferAllocator, FrameBufferId);
AllocatorImpl(VertexArrayAllocator, VertexArrayId);

}    // namespace owlet::gl
