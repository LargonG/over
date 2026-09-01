#pragma once

#include <owlet/gl/memory.h>

namespace owlet::gl {

enum class TextureType : GLenum {
    Texture1D = GL_TEXTURE_1D,
    Texture2D = GL_TEXTURE_2D,
    Texture3D = GL_TEXTURE_3D,
    Cubemap = GL_TEXTURE_CUBE_MAP,
};

enum class TextureWrap : GLint {
    Repeat = GL_REPEAT,
    MirrorRepeat = GL_MIRRORED_REPEAT,
    ToEdge = GL_CLAMP_TO_EDGE,
    MirrorEdge = GL_MIRROR_CLAMP_TO_EDGE,
    ToBorder = GL_CLAMP_TO_BORDER,
};

enum class TextureMinFilter : GLenum {
    Near = GL_NEAREST,
    Linear = GL_LINEAR,
    Near2 = GL_NEAREST_MIPMAP_NEAREST,
    NearLinear = GL_NEAREST_MIPMAP_LINEAR,
    LinearNear = GL_LINEAR_MIPMAP_NEAREST,
    Linear2 = GL_LINEAR_MIPMAP_LINEAR
};

enum class TextureMagFilter : GLenum {
    Near = GL_NEAREST,
    Linear = GL_LINEAR,
};

struct SimpleTextureAllocator : TextureAllocator {
    SimpleTextureAllocator(TextureType type) : m_target(static_cast<GLenum>(type)) {}

    [[nodiscard]] TextureId Alloc(Context*) override;
    void Dealloc(Context*, TextureId) noexcept override;

  private:
    const GLenum m_target;
};

struct Texture2D {
  public:
    Texture2D(Context*, TextureAllocator* ptr = nullptr);

    Texture2D& Alloc(GLenum internal_format, std::tuple<usize, usize> size, usize levels = 0);
    Texture2D& Wrap(std::tuple<TextureWrap, TextureWrap>);
    Texture2D& Filter(std::tuple<TextureMinFilter, TextureMagFilter>);

    Texture2D& Write(int32 level, std::tuple<int32, int32> offset, std::tuple<usize, usize> size, GLenum format,
                     GLenum type, const void* data);

    Texture2D& Bake();

    void AttachUniform(int32 unit);

  private:
    Handler<TextureId> m_handler;
    std::tuple<usize, usize> m_size;
    bool m_is_initialized = false;
};
}    // namespace owlet::gl
