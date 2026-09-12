#pragma once

#include <owlet/gl/allocator.h>
#include <owlet/gl/context.h>
#include <owlet/gl/handler.h>

namespace owlet::gl {

/// @brief Internal realization, don't use as polymorphic type
///        Represents GL object, that has it's own Id inside GPU driver
/// @tparam TypeId
/// @tparam Allocator
template <class TypeId>
struct Object {
  public:
    template <class Allocator, typename F>
    Object(Context* gl, F&& func, Allocator* alloc) : m_handler(alloc ? alloc : func(gl), gl) {}

    [[nodiscard]] TypeId Id() const noexcept { return m_handler.Get(); }
    [[nodiscard]] Context* GL() const noexcept { return m_handler.Context(); }

  protected:
    [[nodiscard]] GLuint RawId() const noexcept { return m_handler.GetRaw(); }

    Handler<TypeId> m_handler;
};

}    // namespace owlet::gl
