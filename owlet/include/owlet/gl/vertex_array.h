#pragma once

#include <glad/gl.h>

#include <owlet/gl/memory.h>
#include <owlet/types.h>

namespace owlet::gl {

constexpr int32 g_attr_position_location = 0;
constexpr int32 g_attr_normal_location = 1;
constexpr int32 g_attr_uv_location = 2;

struct Buffer;

struct VertexArray {
  public:
    VertexArray(Context*, VertexArrayAllocator* = nullptr);

    VertexArray& Enable(int32 location);
    VertexArray& Disable(int32 location);

    VertexArray& Format(int32 location, GLenum type, int32 count, int32 relative_offset);
    VertexArray& Attach(int32 binding_id, Buffer* buf, int64 offset, int32 shift);
    VertexArray& BindFormat(int32 location, int32 binding_id);

    VertexArray& Bind();
    VertexArray& Unbind();

  private:
    Handler<VertexArrayId> m_handler;
};
}    // namespace owlet::gl
