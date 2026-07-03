#pragma once

#include <over/core/opengl/allocators/DefaultRenderBufferAllocator.hpp>
#include <over/core/opengl/targets/RenderBufferTarget.hpp>
#include <over/core/opengl/wrappers/Wrapper.hpp>

namespace over::gl {

template <RenderBufferTarget Target>
class RenderBufferView;

template <class Allocator = DefaultRenderBufferAllocator>
class RenderBufferWrapper final
    : public Wrapper<Allocator, RenderBufferTarget,
                     RenderBufferWrapper<Allocator>, RenderBufferView> {};
}  // namespace over::gl
