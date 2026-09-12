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
struct ShaderAllocator;
struct ProgramAllocator;

// Warning: I'm not sure this is a good idea,
// Invariant is: GladGLContext offset is zero, so gladLoadContext(...) should not affect other parts
struct Context : GladGLContext {
  public:
    struct Allocators {
        std::unique_ptr<BufferAllocator> default_buffer_allocator = nullptr;
        std::unique_ptr<TextureAllocator> default_texture_2d_allocator = nullptr;
        std::unique_ptr<TextureAllocator> default_cube_map_allocator = nullptr;
        std::unique_ptr<TextureAllocator> default_texture_2d_multi_sample_allocator = nullptr;
        std::unique_ptr<RenderBufferAllocator> default_render_buffer_allocator = nullptr;
        std::unique_ptr<FrameBufferAllocator> default_frame_buffer_allocator = nullptr;
        std::unique_ptr<VertexArrayAllocator> default_vertex_array_allocator = nullptr;
        std::unique_ptr<ShaderAllocator> default_shader_allocator = nullptr;
        std::unique_ptr<ProgramAllocator> default_program_allocator = nullptr;
    };

    [[nodiscard]] auto* DefaultBufferAllocator() const noexcept { return m_allocators.default_buffer_allocator.get(); }
    [[nodiscard]] auto* DefaultTexture2DAllocator() const noexcept {
        return m_allocators.default_texture_2d_allocator.get();
    }
    [[nodiscard]] auto* DefaultCubeMapAllocator() const noexcept {
        return m_allocators.default_cube_map_allocator.get();
    }
    [[nodiscard]] auto* DefaultTexture2DMultiSampleAllocator() const noexcept {
        return m_allocators.default_texture_2d_multi_sample_allocator.get();
    }
    [[nodiscard]] auto* DefaultRenderBufferAllocator() const noexcept {
        return m_allocators.default_render_buffer_allocator.get();
    }
    [[nodiscard]] auto* DefaultFrameBufferAllocator() const noexcept {
        return m_allocators.default_frame_buffer_allocator.get();
    }

    [[nodiscard]] auto* DefaultVertexArrayAllocator() const noexcept {
        return m_allocators.default_vertex_array_allocator.get();
    }

    [[nodiscard]] auto* DefaultShaderAllocator() const noexcept { return m_allocators.default_shader_allocator.get(); }

    [[nodiscard]] auto* DefaultProgramAllocator() const noexcept {
        return m_allocators.default_program_allocator.get();
    }

  private:
    Allocators m_allocators;

    friend struct ::owlet::os::Window;
};

struct Version {
    int32 major = 4;
    int32 minor = 6;
};

using Settings = Context::Allocators;

}    // namespace owlet::gl
