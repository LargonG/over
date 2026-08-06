module;

#include <cstddef>
#include <cstdint>

export module owlet.types;

namespace lowl {
// No need for int8 and int16
export using int32 = std::int32_t;
export using int64 = std::int64_t;

// Avoid using unsigned variants: overflow with negative values occurs
// export using uint32 = std::uint32_t;

export using float32 = float;
export using float64 = double;

export using byte = std::byte;

export using usize = std::size_t;

}    // namespace lowl
