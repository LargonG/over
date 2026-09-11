#include <owlet/gl/layout.h>

#include <owlet/debug/core.h>
#include <owlet/debug/log.h>
#include <owlet/gl/texture.h>
#include <owlet/gl/vertex_array.h>

#include <fmt/format.h>

namespace owlet::gl {
Layout::Layout() : m_vertex_array(nullptr), m_textures(), m_previous(nullptr) {}

Layout& Layout::Set(VertexArray& vao) {
    m_vertex_array = &vao;

    return *this;
}
Layout& Layout::Set(Texture& tex, int32 unit) {
    debug::Require(0 <= unit && unit < m_textures.size(), "Unit must in range of 0..16");

    m_textures[unit] = &tex;

    return *this;
}

Layout& Layout::Unset(int32 unit) {
    m_textures[unit] = nullptr;

    return *this;
}

void Layout::Begin(Layout* previous) {
    m_previous = previous;

    Activate();
}

void Layout::End() {
    if (m_previous) {
        m_previous->Activate();
    }
}

void Layout::Activate() {
    debug::Require(m_vertex_array != nullptr, "Vertex Array must be initialized");
    m_vertex_array->Bind();

    for (int32 i = 0; i < m_textures.size(); i++) {
        if constexpr (debug::g_owlet_debug) {
            if (m_textures[i] && m_textures[i]->GL() != m_vertex_array->GL()) {
                debug::DefaultLogger()->Log(debug::Level::Warning,
                                            fmt::format("Different gl contexts: {} {}",

                                                        reinterpret_cast<void*>(m_vertex_array->GL()),
                                                        reinterpret_cast<void*>(m_textures[i]->GL())));
            }
        }
        Texture::AttachUniform(m_vertex_array->GL(), i, (m_textures[i] ? m_textures[i]->Id() : TextureId::Null));
    }

    m_vertex_array->Unbind();
}

}    // namespace owlet::gl
