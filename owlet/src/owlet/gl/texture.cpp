#include <owlet/gl/texture.h>

#include <tuple>

#include <glad/gl.h>
#include <owlet/debug/core.h>
#include <owlet/debug/gl.h>
#include <owlet/gl/context.h>
#include <owlet/types.h>

namespace owlet::gl {

void Texture::AttachUniform(int32 unit) {
    auto* gl = GL();

    AttachUniform(gl, unit, m_handler.Get());
}

void Texture::AttachUniform(Context* gl, int32 unit, TextureId id) {
    gl->BindTextureUnit(static_cast<GLuint>(unit), static_cast<GLuint>(id));
    debug::GLCheckError(gl);
}

TextureId SimpleTextureAllocator::Alloc(Context* gl) {
    GLuint res = 0;

    gl->CreateTextures(m_target, 1, &res);
    debug::GLCheckError(gl);

    return TextureId(res);
}

void SimpleTextureAllocator::Dealloc(Context* gl, TextureId id) noexcept {
    GLuint rid = static_cast<GLuint>(id);
    debug::Assert(gl->IsTexture(rid));

    gl->DeleteTextures(1, &rid);
    debug::GLCheckError(gl);
}

Texture2D::Texture2D(Context* ctx, TextureAllocator* alloc)
    : Texture(
          ctx, [](Context* gl) { return gl->DefaultTexture2DAllocator(); }, alloc),
      m_format(GL_RGB32F),
      m_size(0, 0),
      m_levels(0),
      m_initialized(false) {}

Texture2D& Texture2D::Alloc(GLenum internal_format, std::tuple<usize, usize> size, usize levels) {
    debug::Require(!m_initialized, "Immutable texture storage cannot be reallocated, create new texture");

    auto* gl = GL();
    auto [size_x, size_y] = size;
    gl->TextureStorage2D(RawId(), static_cast<GLsizei>(levels), internal_format, static_cast<GLsizei>(size_x),
                         static_cast<GLsizei>(size_y));
    debug::GLCheckError(gl);

    m_format = internal_format;
    m_size = size;
    m_levels = levels;
    m_initialized = true;

    return *this;
}

Texture2D& Texture2D::Wrap(TextureWrap wrap_s, TextureWrap wrap_t) {
    auto* gl = GL();

    gl->TextureParameteri(RawId(), GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap_s));
    debug::GLCheckError(gl);

    gl->TextureParameteri(RawId(), GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap_t));
    debug::GLCheckError(gl);

    return *this;
}

Texture2D& Texture2D::Filter(TextureMinFilter min, TextureMagFilter mag) {
    auto* gl = GL();

    gl->TextureParameteri(RawId(), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
    debug::GLCheckError(gl);

    gl->TextureParameteri(RawId(), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag));
    debug::GLCheckError(gl);

    return *this;
}

Texture2D& Texture2D::Write(int32 level, std::tuple<int32, int32> offset, std::tuple<usize, usize> size, GLenum format,
                            GLenum type, const void* data) {
    auto* gl = GL();

    auto [m_size_x, m_size_y] = m_size;
    auto [offset_x, offset_y] = offset;
    auto [size_x, size_y] = size;

    debug::Assert(0 <= offset_x && 0 <= offset_y && offset_x <= m_size_x && offset_y <= m_size_y);
    debug::Assert(offset_x + size_x <= m_size_x && offset_y + size_y <= m_size_y);
    debug::Assert(0 <= level && level < m_levels);

    gl->TextureSubImage2D(RawId(), static_cast<GLint>(level), static_cast<GLint>(offset_x),
                          static_cast<GLint>(offset_y), static_cast<GLsizei>(size_x), static_cast<GLsizei>(size_y),
                          format, type, data);
    debug::GLCheckError(gl);

    return *this;
}

Texture2D& Texture2D::Bake() {
    auto* gl = GL();

    gl->GenerateTextureMipmap(RawId());
    debug::GLCheckError(gl);

    return *this;
}

Texture2D& Texture2D::AttachUniform(int32 unit) {
    Texture::AttachUniform(unit);

    return *this;
}

CubeMap::CubeMap(Context* gl, TextureAllocator* alloc)
    : Texture(
          gl, [](Context* gl) { return gl->DefaultCubeMapAllocator(); }, alloc),
      m_size(0),
      m_format(GL_RGB32F),
      m_levels(0),
      m_initialized(false) {}

CubeMap& CubeMap::Alloc(GLenum internal_format, usize size, usize levels) {
    debug::Require(!m_initialized, "Immutable storage cannot be reallocated, create new cubemap");
    auto* gl = GL();

    gl->TextureStorage2D(RawId(), static_cast<GLsizei>(levels), internal_format, static_cast<GLsizei>(size),
                         static_cast<GLsizei>(size));
    debug::GLCheckError(gl);

    m_format = internal_format;
    m_size = size;
    m_levels = levels;
    m_initialized = true;

    return *this;
}

CubeMap& CubeMap::Write(int32 face, int32 level, std::tuple<int32, int32> offset, std::tuple<usize, usize> size,
                        GLenum format, GLenum type, const void* data) {
    debug::Assert(0 <= face && face < 6, "There are 6 faces for cubemap");

    auto* gl = GL();

    auto [offset_x, offset_y] = offset;
    auto [size_x, size_y] = size;

    debug::Assert(0 <= offset_x && 0 <= offset_y && offset_x <= m_size && offset_y <= m_size);
    debug::Assert(offset_x + size_x <= m_size && offset_y + size_y <= m_size);
    debug::Assert(0 <= level && level < m_levels);

    gl->TextureSubImage3D(RawId(), level, static_cast<GLint>(offset_x), static_cast<GLint>(offset_y),
                          static_cast<GLint>(face), static_cast<GLsizei>(size_x), static_cast<GLsizei>(size_y),
                          static_cast<GLsizei>(1), format, type, data);
    debug::GLCheckError(gl);

    return *this;
}

CubeMap& CubeMap::Wrap(TextureWrap wrap_s, TextureWrap wrap_t) {
    auto* gl = GL();

    gl->TextureParameteri(RawId(), GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap_s));
    debug::GLCheckError(gl);

    gl->TextureParameteri(RawId(), GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap_t));
    debug::GLCheckError(gl);

    return *this;
}

CubeMap& CubeMap::Filter(TextureMinFilter min, TextureMagFilter mag) {
    auto* gl = GL();

    auto id = RawId();
    gl->TextureParameteri(id, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
    debug::GLCheckError(gl);

    gl->TextureParameteri(id, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag));
    debug::GLCheckError(gl);

    return *this;
}

CubeMap& CubeMap::Bake() {
    auto* gl = GL();

    gl->GenerateTextureMipmap(RawId());
    debug::GLCheckError(gl);

    return *this;
}

CubeMap& CubeMap::AttachUniform(int32 unit) {
    Texture::AttachUniform(unit);

    return *this;
}

Texture2DMultiSample::Texture2DMultiSample(Context* gl, TextureAllocator* alloc)
    : Object(
          gl, [](Context* gl) { return gl->DefaultTexture2DMultiSampleAllocator(); }, alloc),
      m_format(GL_RGB32F),
      m_size(0, 0),
      m_samples(0),
      m_fixed(false),
      m_initialized(false) {}

Texture2DMultiSample& Texture2DMultiSample::Alloc(GLenum internal_format, std::tuple<usize, usize> size, usize samples,
                                                  bool fixed) {
    debug::Require(!m_initialized, "Immutable storage cannot be reallocated, create new multisample texture");
    auto* gl = GL();

    auto [size_x, size_y] = size;

    gl->TextureStorage2DMultisample(RawId(), static_cast<GLsizei>(samples), internal_format,
                                    static_cast<GLsizei>(size_x), static_cast<GLsizei>(size_y),
                                    static_cast<GLboolean>(fixed));
    debug::GLCheckError(gl);

    m_format = internal_format;
    m_size = size;
    m_samples = samples;
    m_fixed = fixed;
    m_initialized = true;

    return *this;
}

}    // namespace owlet::gl
