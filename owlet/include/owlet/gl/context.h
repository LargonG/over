#pragma once

#include <memory>

#include <glad/gl.h>

#include <owlet/types.h>

namespace owlet::os {
struct Window;
}

namespace owlet::gl {

struct BufferAllocator;
struct TextureAllocator;
struct RenderBufferAllocator;
struct FrameBufferAllocator;
struct VertexArrayAllocator;

// Warning: I'm not sure this is a good idea,
// Invariant is: GladGLContext offset is zero, so gladLoadContext(...) should not affect other parts
struct Context : GladGLContext {
  public:
    auto* DefaultBufferAllocator() const noexcept { return m_default_buffer_allocator.get(); }
    auto* DefaultTexture2DAllocator() const noexcept { return m_default_texture_2d_allocator.get(); }
    auto* DefaultCubeMapAllocator() const noexcept { return m_default_cube_map_allocator.get(); }
    auto* DefaultTexture2DMultiSampleAllocator() const noexcept {
        return m_default_texture_2d_multi_sample_allocator.get();
    }
    auto* DefaultRenderBufferAllocator() const noexcept { return m_default_render_buffer_allocator.get(); }
    auto* DefaultFrameBufferAllocator() const noexcept { return m_default_frame_buffer_allocator.get(); }

    auto* DefaultVertexArrayAllocator() const noexcept { return m_default_vertex_array_allocator.get(); }

  private:
    std::unique_ptr<BufferAllocator> m_default_buffer_allocator;
    std::unique_ptr<TextureAllocator> m_default_texture_2d_allocator;
    std::unique_ptr<TextureAllocator> m_default_cube_map_allocator;
    std::unique_ptr<TextureAllocator> m_default_texture_2d_multi_sample_allocator;
    std::unique_ptr<RenderBufferAllocator> m_default_render_buffer_allocator;
    std::unique_ptr<FrameBufferAllocator> m_default_frame_buffer_allocator;
    std::unique_ptr<VertexArrayAllocator> m_default_vertex_array_allocator;

    friend struct ::owlet::os::Window;
};

struct Version {
    int32 major = 4;
    int32 minor = 6;
};

struct Settings {
    std::unique_ptr<BufferAllocator> default_buffer_allocator = nullptr;
    std::unique_ptr<TextureAllocator> default_texture_2d_allocator = nullptr;
    std::unique_ptr<TextureAllocator> default_cube_map_allocator = nullptr;
    std::unique_ptr<TextureAllocator> default_texture_2d_multi_sample_allocator = nullptr;
    std::unique_ptr<RenderBufferAllocator> default_render_buffer_allocator = nullptr;
    std::unique_ptr<FrameBufferAllocator> default_frame_buffer_allocator = nullptr;
    std::unique_ptr<VertexArrayAllocator> default_vertex_array_allocator = nullptr;
};

}    // namespace owlet::gl
