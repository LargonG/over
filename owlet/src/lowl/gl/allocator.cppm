export module owlet.gl:allocator;

import owlet.debug;

namespace lowl::gl {

/// <summary>
/// Internal GL Allocator interface, don't use it as polymorphic type
/// </summary>
/// <typeparam name="T">Type of Id, example: Buffer::Id</typeparam>
template <class T>
class GLAllocator {
  private:
    // For some reason noexcept(!lowl::Debug) does not work without this line
    static constexpr bool Debug = lowl::Debug;

  public:
    // And now it know... finally, wat?
    // TODO: why is New `noexcept`? It should be able to throw exception, when there is not enough memory
    [[nodiscard]] virtual auto New() noexcept(!lowl::Debug) -> T = 0;
    virtual auto Delete(T) noexcept(!lowl::Debug) -> void = 0;
};
}    // namespace lowl::gl
