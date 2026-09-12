#pragma once

#include <span>

#include <glad/gl.h>
#include <owlet/collections.h>

namespace owlet::gl {

struct Context;

/// @brief container of OpenGL configuration setters
/// such as depth test, stencil test, cull face, depth clamp and others
struct State {
  public:
    State(Context* context);

    void Begin(State* previous);
    void End();

    State& Enable(std::span<const GLenum>);
    State& Enable(std::initializer_list<GLenum> lst) { return Enable(std::span<const GLenum>(lst.begin(), lst.end())); }

    State& Disable(std::span<const GLenum>);
    State& Disable(std::initializer_list<GLenum> lst) {
        return Disable(std::span<const GLenum>(lst.begin(), lst.end()));
    }

    State& Untrack(std::span<const GLenum>);
    State& Untrack(std::initializer_list<GLenum> lst) {
        return Untrack(std::span<const GLenum>(lst.begin(), lst.end()));
    }

    Context* GL() const noexcept { return m_gl; }

  private:
    void Activate();
    void Deactivate();

    Context* m_gl;

    State* m_previous;
    hashset<GLenum> m_changed;

    hashset<GLenum> m_force_enable;
    hashset<GLenum> m_force_disable;

    static bool Enabled(State* state, GLenum value);
    static bool Disabled(State* state, GLenum value);
};
}    // namespace owlet::gl
