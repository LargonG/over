#include <owlet/gl/state.h>

#include <span>
#include <utility>

#include <glad/gl.h>
#include <owlet/gl/context.h>

namespace owlet::gl {

bool State::Enabled(State* state, GLenum value) {
    return state && state->m_force_enable.contains(value);
}

bool State::Disabled(State* state, GLenum value) {
    return state && state->m_force_disable.contains(value);
}

State::State(Context* gl) : m_gl(gl), m_previous(nullptr), m_changed(), m_force_enable(), m_force_disable() {}

State& State::Enable(std::span<const GLenum> funcs) {
    for (const auto& x : funcs) {
        m_force_enable.insert(x);
        m_force_disable.erase(x);
    }

    return *this;
}

State& State::Disable(std::span<const GLenum> funcs) {
    for (const auto& x : funcs) {
        m_force_enable.erase(x);
        m_force_disable.insert(x);
    }

    return *this;
}

State& State::Untrack(std::span<const GLenum> funcs) {
    for (const auto& x : funcs) {
        m_force_enable.erase(x);
        m_force_disable.erase(x);
    }

    return *this;
}

void State::Begin(State* previous) {
    m_previous = previous;
    Activate();
}

void State::End() {
    Deactivate();
}

void State::Activate() {
    m_changed.clear();
    for (const auto& x : m_force_enable) {
        if (Disabled(m_previous, x) || !m_gl->IsEnabled(x)) {
            m_changed.insert(x);
        }
        m_gl->Enable(x);
    }

    for (const auto& x : m_force_disable) {
        if (Enabled(m_previous, x) || m_gl->IsEnabled(x)) {
            m_changed.insert(x);
        }
        m_gl->Disable(x);
    }
}

void State::Deactivate() {
    for (const auto& x : m_force_enable) {
        if (m_changed.contains(x)) {
            m_gl->Disable(x);
        }
    }
    for (const auto& x : m_force_disable) {
        if (m_changed.contains(x)) {
            m_gl->Enable(x);
        }
    }
}

}    // namespace owlet::gl
