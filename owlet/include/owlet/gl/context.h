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

// Warning: I'm not sure this is a good idea,
// Invariant is: GladGLContext offset is zero, so gladLoadContext(...) should not affect other parts
struct Context : GladGLContext {
  public:
    auto* DefaultBufferAllocator() const noexcept { return m_default_buffer_allocator.get(); }
    auto* DefaultTexture2dAllocator() const noexcept { return m_default_texture2d_allocator.get(); }

  private:
    std::unique_ptr<BufferAllocator> m_default_buffer_allocator;

    std::unique_ptr<TextureAllocator> m_default_texture2d_allocator;

    friend struct ::owlet::os::Window;
};

struct Version {
    int32 major = 4;
    int32 minor = 6;
};

struct Settings {
    std::unique_ptr<BufferAllocator> default_buffer_allocator = std::unique_ptr<BufferAllocator>(nullptr);
};

}    // namespace owlet::gl
