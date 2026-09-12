#pragma once

#include <memory>

#include <owlet/gl/allocator.h>
#include <owlet/gl/context.h>

namespace owlet::gl {

template <class Id>
struct Handler {
  private:
    using Alloc = Allocator<Id>;

    constexpr static Id null_value = Id::Null;

  public:
    Handler(Alloc* alloc, Context* context) : m_alloc(alloc), m_context(context), m_id(null_value) {}

    Handler(const Handler&) = delete;

    Handler(Handler&& other) noexcept : Handler() { *this = std::move(other); }

    Handler& operator=(Handler&& other) noexcept {
        std::swap(m_alloc, other.m_alloc);
        std::swap(m_id, other.m_id);

        return *this;
    }

    ~Handler() { Free(); }

    [[nodiscard]] auto Get() const noexcept {
        if (m_id == null_value) {
            m_id = Id(m_alloc->Alloc(m_context));
        }
        return m_id;
    }

    [[nodiscard]] auto GetRaw() const noexcept { return static_cast<GLuint>(Get()); }

    [[nodiscard]] auto Context() const noexcept { return m_context; }

  private:
    Handler() noexcept : m_alloc(nullptr), m_id(null_value) {}

    void Free() {
        if (m_id == null_value) {
            return;
        }

        m_alloc->Dealloc(m_context, m_id);
        m_id = null_value;
    }

    Alloc* m_alloc;
    gl::Context* m_context;
    mutable Id m_id;
};

}    // namespace owlet::gl
