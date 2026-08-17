#pragma once

#include <cstddef>
#include <cstdint>

namespace owlet {
// No need for 8-16 variants
using int32 = std::int32_t;
using int64 = std::int64_t;

// Avoid using unsigned variants of these types,
// it causes implicit bugs, like: -1 + uint32 = blow

// Use as indexing of a container
using usize = std::size_t;

using float32 = float;

using anytype = void*;

// No need of using double precision in render-engine
// using float64 = double;
}    // namespace owlet
