#pragma once

#include <array>
#include <optional>

#include <owlet/types.h>

namespace owlet::gl {

struct VertexArray;
struct Texture;

struct Layout {
  public:
    Layout();

    Layout& Set(VertexArray&);
    Layout& Set(Texture& tex, int32 unit);

    Layout& Unset(int32 unit);

    void Begin(Layout*);
    void End();

  private:
    void Activate();

    Layout* m_previous;

    VertexArray* m_vertex_array;
    std::array<Texture*, 16> m_textures;
};
}    // namespace owlet::gl
