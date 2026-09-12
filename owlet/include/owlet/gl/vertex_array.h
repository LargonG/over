#pragma once

#include <glad/gl.h>

#include <initializer_list>
#include <span>

#include <owlet/gl/memory.h>
#include <owlet/types.h>

namespace owlet::gl {

constexpr int32 g_attr_position_location = 0;
constexpr int32 g_attr_normal_location = 1;
constexpr int32 g_attr_uv_location = 2;

struct IndexBuffer;

struct Buffer;

struct SimpleVertexArrayAllocator : VertexArrayAllocator {
    SimpleVertexArrayAllocator() {}

    [[nodiscard]] VertexArrayId Alloc(Context*) override;
    void Dealloc(Context*, VertexArrayId) noexcept override;
};

struct VertexArray : gl::Object<VertexArrayId> {
  public:
    VertexArray(Context*, VertexArrayAllocator* = nullptr);

    VertexArray& Enable(int32 location);
    VertexArray& Disable(int32 location);

    VertexArray& Format(int32 location, GLenum type, int32 count, int32 relative_offset);
    VertexArray& Attach(int32 binding_id, Buffer& buf, int64 offset, int32 shift);
    VertexArray& BindFormat(int32 binding_id, int32 location);
    VertexArray& BindFormats(int32 binding_id, std::span<const int32> locations);
    VertexArray& BindFormats(int32 binding_id, std::initializer_list<int32> locations) {
        return BindFormats(binding_id, std::span<const int32>(locations.begin(), locations.end()));
    }

    VertexArray& AttachIndex(IndexBuffer&);

    VertexArray& Bind();
    VertexArray& Unbind();
};
}    // namespace owlet::gl
