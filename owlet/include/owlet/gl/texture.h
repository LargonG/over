#pragma once

#include <owlet/gl/memory.h>

namespace owlet::gl {

enum class TextureType : GLenum {
    Texture2D = GL_TEXTURE_2D,
    CubeMap = GL_TEXTURE_CUBE_MAP,
    Texture2DMultiSample = GL_TEXTURE_2D_MULTISAMPLE,
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
    explicit SimpleTextureAllocator(TextureType type) : m_target(static_cast<GLenum>(type)) {}

    [[nodiscard]] TextureId Alloc(Context*) override;
    void Dealloc(Context*, TextureId) noexcept override;

  private:
    const GLenum m_target;
};

/// @brief Abstract "interface" for textures, do not use it as polymorphic type
struct Texture : Object<TextureId, TextureAllocator> {
  public:
    template <typename F>
    Texture(Context* gl, F&& func, TextureAllocator* alloc) : Object(gl, std::forward<F>(func), alloc) {}

    void AttachUniform(int32 unit);

    static void AttachUniform(Context* gl, int32 unit, TextureId id);
};

struct Texture2D : Texture {
  public:
    explicit Texture2D(Context*, TextureAllocator* = nullptr);

    Texture2D& Alloc(GLenum internal_format, std::tuple<usize, usize> size, usize levels = 1);
    Texture2D& Wrap(TextureWrap, TextureWrap);
    Texture2D& Filter(TextureMinFilter, TextureMagFilter);

    Texture2D& Write(int32 level, std::tuple<int32, int32> offset, std::tuple<usize, usize> size, GLenum format,
                     GLenum type, const void* data);

    Texture2D& Bake();

    Texture2D& AttachUniform(int32 unit);

  private:
    GLenum m_format;
    std::tuple<usize, usize> m_size;
    usize m_levels;

    bool m_initialized;
};

struct CubeMap : Texture {
  public:
    explicit CubeMap(Context*, TextureAllocator* = nullptr);

    CubeMap& Alloc(GLenum internal_format, usize size, usize levels = 1);
    CubeMap& Write(int32 face, int32 level, std::tuple<int32, int32> offset, std::tuple<usize, usize> size,
                   GLenum format, GLenum type, const void* data);

    CubeMap& Wrap(TextureWrap, TextureWrap);
    CubeMap& Filter(TextureMinFilter, TextureMagFilter);

    CubeMap& Bake();

    CubeMap& AttachUniform(int32 unit);

  private:
    GLenum m_format;
    usize m_size;
    usize m_levels;

    bool m_initialized;
};

struct Texture2DMultiSample : Object<TextureId, TextureAllocator> {
  public:
    explicit Texture2DMultiSample(Context*, TextureAllocator* = nullptr);

    Texture2DMultiSample& Alloc(GLenum internal_format, std::tuple<usize, usize> size, usize samples = 1,
                                bool fixed = true);

  private:
    GLenum m_format;
    std::tuple<usize, usize> m_size;
    usize m_samples;
    bool m_fixed;

    bool m_initialized;
};

}    // namespace owlet::gl
