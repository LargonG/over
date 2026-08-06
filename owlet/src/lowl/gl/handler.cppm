module;

#include <memory>

#include <glad/gl.h>

export module owlet.gl:handler;

import owlet.debug;

namespace lowl::gl {

// Handler contains gl object ID and controls it's lifetime
template <class Id, class Allocator>
class Handler final {
  public:
    explicit Handler(Allocator&, Id id = Id::Null) noexcept;

    Handler(const Handler&) = delete;
    auto operator=(const Handler&) -> Handler& = delete;

    Handler(Handler&&) noexcept;
    auto operator=(Handler&&) noexcept -> Handler&;

    ~Handler();

    [[nodiscard]] auto Get() noexcept -> Id;
    [[nodiscard]] auto GetRaw() noexcept -> GLuint;

  private:
    // Free is private because obj should be deleted only when the Handler is deleted
    auto Free() noexcept -> void;

    // GPU memory allocation are rare operations, so, no need to optimize
    Allocator* m_alloc;

    Id m_ptr;
};
}    // namespace lowl::gl

namespace lowl::gl {

#define HandlerCtor                      \
    template <class Id, class Allocator> \
    Handler<Id, Allocator>
#define HandlerImpl                      \
    template <class Id, class Allocator> \
    auto Handler<Id, Allocator>

#pragma region Handler impl

HandlerCtor::Handler(Allocator& alloc, Id id) noexcept : m_alloc(&alloc), m_ptr(id) {}

HandlerCtor::Handler(Handler&& other) noexcept : m_alloc(nullptr), m_ptr(Id::Null) {
    *this = std::move(other);
}

HandlerImpl::operator=(Handler&& other) noexcept -> Handler& {
    if (this == &other) {
        return *this;
    }

    Assert(m_ptr != other.m_ptr);
    Free();

    m_ptr = std::exchange(other.m_ptr, Id::Null);
    m_alloc = std::exchange(other.m_alloc, nullptr);

    return *this;
}

HandlerCtor::~Handler() {
    Free();
}

HandlerImpl::Get() noexcept -> Id {
    Assert(m_alloc != nullptr);

    if (m_ptr == Id::Null) {
        m_ptr = m_alloc->New();
    }
    return m_ptr;
}

HandlerImpl::GetRaw() noexcept -> GLuint {
    return static_cast<GLuint>(Get());
}

HandlerImpl::Free() noexcept -> void {
    if (m_ptr == Id::Null) {
        return;
    }
    Assert(m_alloc != nullptr);

    m_alloc->Delete(m_ptr);
    m_ptr = Id::Null;
}

#pragma endregion
}    // namespace lowl::gl
