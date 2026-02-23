#pragma once

#include <cstdint>
#include <memory>

namespace over {
using int64 = int64_t;
using int32 = int32_t;
using int16 = int16_t;
using int8 = int8_t;

using uint64 = uint64_t;
using uint32 = uint32_t;
using uint16 = uint16_t;
using uint8 = uint8_t;

using float64 = double;
using float32 = float;

using usize = std::size_t;
using ssize = std::ptrdiff_t;

static_assert(sizeof(int64) == 8, "int 64 bits is 8 bytes");
static_assert(sizeof(int32) == 4, "int 32 bits is 4 bytes");
static_assert(sizeof(int16) == 2, "int 16 bits is 2 bytes");
static_assert(sizeof(int8) == 1, "int 8 bits is 1 byte");

static_assert(sizeof(uint64) == 8, "uint 64 bits is 8 bytes");
static_assert(sizeof(uint32) == 4, "uint 32 bits is 4 bytes");
static_assert(sizeof(uint16) == 2, "uint 16 bits is 2 bytes");
static_assert(sizeof(uint8) == 1, "uint 8 bits is 1 byte");

static_assert(sizeof(float64) == 8, "float 64 bits is 8 bytes, double");
static_assert(sizeof(float32) == 4, "float 32 bits is 4 bytes, float");
}  // namespace over
