#include <owlet/gl/texture.h>

#include <tuple>

#include <glad/gl.h>
#include <owlet/debug/core.h>
#include <owlet/debug/gl.h>
#include <owlet/gl/context.h>
#include <owlet/types.h>

namespace owlet::gl {
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
    : m_handler(alloc ? alloc : ctx->DefaultTexture2dAllocator(), ctx), m_size(0, 0) {}

Texture2D& Texture2D::Alloc(GLenum internal_format, std::tuple<usize, usize> size, usize levels) {
    debug::Require(!m_is_initialized, "Immutable texture storage cannot be reallocated, create new texture");

    auto [size_x, size_y] = size;
    m_handler.Context()->TextureStorage2D(m_handler.GetRaw(), static_cast<GLsizei>(levels), internal_format,
                                          static_cast<GLsizei>(size_x), static_cast<GLsizei>(size_y));
    debug::GLCheckError(m_handler.Context());

    m_is_initialized = true;
    m_size = size;

    return *this;
}

Texture2D& Texture2D::Wrap(std::tuple<TextureWrap, TextureWrap> wrap) {
    auto [wrap_r, wrap_s] = wrap;

    auto* gl = m_handler.Context();

    gl->TextureParameteri(m_handler.GetRaw(), GL_TEXTURE_WRAP_R, static_cast<GLint>(wrap_r));
    debug::GLCheckError(gl);

    gl->TextureParameteri(m_handler.GetRaw(), GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap_s));
    debug::GLCheckError(gl);

    return *this;
}

Texture2D& Texture2D::Filter(std::tuple<TextureMinFilter, TextureMagFilter> filter) {
    auto [min, mag] = filter;
    auto* gl = m_handler.Context();

    gl->TextureParameteri(m_handler.GetRaw(), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
    debug::GLCheckError(gl);

    gl->TextureParameteri(m_handler.GetRaw(), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag));
    debug::GLCheckError(gl);

    return *this;
}

Texture2D& Texture2D::Write(int32 level, std::tuple<int32, int32> offset, std::tuple<usize, usize> size, GLenum format,
                            GLenum type, const void* data) {
    auto* gl = m_handler.Context();

    auto [offset_x, offset_y] = offset;
    auto [size_x, size_y] = size;

    gl->TextureSubImage2D(m_handler.GetRaw(), static_cast<GLint>(level), static_cast<GLint>(offset_x),
                          static_cast<GLint>(offset_y), static_cast<GLsizei>(size_x), static_cast<GLsizei>(size_y),
                          format, type, data);
    debug::GLCheckError(gl);

    return *this;
}

Texture2D& Texture2D::Bake() {
    auto* gl = m_handler.Context();

    gl->GenerateTextureMipmap(m_handler.GetRaw());
    debug::GLCheckError(gl);

    return *this;
}

void Texture2D::AttachUniform(int32 unit) {
    auto gl = m_handler.Context();
    gl->BindTextureUnit(static_cast<GLuint>(unit), m_handler.GetRaw());
    debug::GLCheckError(gl);
}

}    // namespace owlet::gl
