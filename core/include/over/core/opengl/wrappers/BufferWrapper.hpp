#pragma once

#include <array>
#include <cassert>
#include <type_traits>
#include <vector>

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/Address.hpp>
#include <over/core/opengl/Binded.hpp>
#include <over/core/opengl/allocators/DefaultBufferAllocator.hpp>
#include <over/core/opengl/targets/BufferTarget.hpp>
#include <over/core/opengl/wrappers/Wrapper.hpp>

#include <fmt/core.h>
#include <glm/glm.hpp>

namespace over::gl {

template <BufferTarget Target>
class BufferView;

template <class Allocator = DefaultBufferAllocator>
class BufferWrapper final
    : public Wrapper<Allocator, BufferTarget,
              BufferWrapper<Allocator>,
                     BufferView> {};

}  // namespace over::gl
