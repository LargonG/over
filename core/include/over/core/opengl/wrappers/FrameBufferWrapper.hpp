#pragma once

#include <over/core/opengl/allocators/DefaultFrameBufferAllocator.hpp>
#include <over/core/opengl/targets/FrameBufferTarget.hpp>
#include <over/core/opengl/wrappers/Wrapper.hpp>

namespace over::gl {

template <FrameBufferTarget>
class FrameBufferView;

template <class Allocator = DefaultFrameBufferAllocator>
class FrameBufferWrapper final
    : public Wrapper<Allocator, FrameBufferTarget,
                     FrameBufferWrapper<Allocator>, FrameBufferView> {};
}  // namespace over::gl
