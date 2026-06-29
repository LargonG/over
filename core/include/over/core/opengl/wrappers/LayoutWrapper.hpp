#pragma once

#include <vector>

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>
#include <over/core/opengl/allocators/DefaultLayoutAllocator.hpp>
#include <over/core/opengl/targets/LayoutTarget.hpp>
#include <over/core/opengl/wrappers/Wrapper.hpp>

namespace over::gl {

template <LayoutTarget Target>
class LayoutView;

template <class Allocator = DefaultLayoutAllocator>
class LayoutWrapper : public Wrapper<Allocator, LayoutTarget,
                                     LayoutWrapper<Allocator>, LayoutView> {};

}  // namespace over::gl
