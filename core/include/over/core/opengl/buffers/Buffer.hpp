#pragma once

#include <array>
#include <cassert>
#include <type_traits>
#include <vector>

#include <over/core/Includes.hpp>
#include <over/core/Types.hpp>

#include <over/core/opengl/Binded.hpp>
#include <over/core/opengl/allocators/DefaultBufferAllocator.hpp>

#include <fmt/core.h>
#include <glm/glm.hpp>

namespace over {

constexpr std::array<GLenum, 9> bufferTargets = {
    GL_ARRAY_BUFFER,      GL_COPY_READ_BUFFER,
    GL_COPY_WRITE_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
    GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER,
    GL_TEXTURE_BUFFER,    GL_TRANSFORM_FEEDBACK_BUFFER,
    GL_UNIFORM_BUFFER};

constexpr bool IsAvailableTarget(GLenum target) {
  for (usize i = 0; i < bufferTargets.size(); i++) {
    if (target == bufferTargets[i]) {
      return true;
    }
  }
  return false;
}

template <GLenum Target>
class Buffer : Binded<Buffer<Target>> {
  static_assert(IsAvailableTarget(Target), "Invalid target");

 public:
  Buffer() : _id(0) {}
  Buffer(usize count, const ubyte* data, GLenum usage) : Buffer() {
    Alloc();
    Reserve(count, data, usage);
  }
  template <class T, usize SZ>
  Buffer(std::array<T, SZ>& data, GLenum usage)
      : Buffer(sizeof(T) * SZ, reinterpret_cast<const ubyte*>(data.data()),
               usage) {}

  template <class T>
  Buffer(std::vector<T>& v, GLenum usage)
      : Buffer(sizeof(T) * v.size(), v.data(), usage) {}

  Buffer(const Buffer&) = default;
  Buffer(Buffer&&) noexcept = default;

  Buffer& operator=(const Buffer& other) {
    if (this == &other) {
      return *this;
    }

    if (0 == other._id) {
      Free();
      return *this;
    }

    other.Bind();
    GLuint otherSize = other.GetParameter(GL_BUFFER_SIZE);
    GLuint otherUsage = other.GetParameter(GL_BUFFER_USAGE);
    other.Unbind();

    Alloc();

    Use([&] {
      if (GetParameter(GL_BUFFER_SIZE) != otherSize) {
        Reserve(otherSize, nullptr, otherUsage);
      }
    });

    other.Copy(*this, 0, 0, otherSize);

    return *this;
  }

  Buffer& operator=(Buffer&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    _id = std::exchange(other._id, 0);

    return *this;
  }

  ~Buffer() { Free(); }

  void Alloc() {
    if (0 == _id) {
      _id = DefaultBufferAllocator().Alloc();
    }
  }

  void Reserve(usize count, const ubyte* data, GLenum usage) {
    glBufferData(Target, static_cast<GLsizeiptr>(count),
                 static_cast<const void*>(data), usage);
  }

  void Clear(GLenum usage) {
    if (0 == _id) {
      return;
    }

    glBufferData(Target, 0, nullptr, usage);
  }

  void Free() {
    if (0 == _id) {
      return;
    }

    DefaultBufferAllocator().Dealloc(_id);
    _id = 0;
  }

  void Read(ssize offset, usize count, ubyte* dest) const {
    Map<GL_READ_ONLY>([&](const ubyte* ptr) {
      std::copy(ptr + offset, ptr + offset + count, dest);
    });
  }

  void Write(ssize offset, usize count, const ubyte* data) {
    glBufferSubData(Target, static_cast<GLintptr>(offset),
                    static_cast<GLsizeiptr>(count),
                    static_cast<const void*>(data));
  }

  template <GLenum OtherTarget>
  void Copy(Buffer<OtherTarget>& destination, ssize readOffset,
            ssize writeOffset, usize size) const {
    assert(0 != _id);
    assert(0 != destination._id);

    if (_id == destination._id) {
      return;
    }

    constexpr GLenum readTarget =
        Target == OtherTarget ? GL_COPY_READ_BUFFER : Target;
    constexpr GLenum writeTarget =
        Target == OtherTarget ? GL_COPY_WRITE_BUFFER : OtherTarget;

    Use(readTarget, [&] {
      destination.Use(writeTarget, [&] {
        glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset,
                            size);
      });
    });
  }

  void Bind(GLenum as = Target) const noexcept {
    assert(0 != _id);
    glBindBuffer(as, _id);
  }

  void Unbind(GLenum as = Target) const noexcept {
    // BUG: set as 0, but should set to previous binded
    assert(0 != _id);
    glBindBuffer(as, 0);
  }

  GLint GetParameter(GLenum value) const {
    GLint param = 0;
    glGetBufferParameteriv(Target, value, &param);
    return param;
  }

  bool IsAllocated() const noexcept { return 0 != _id; }

  template <GLenum Access, typename F,
            typename = std::enable_if_t<Access == GL_WRITE_ONLY ||
                                        Access == GL_READ_WRITE>>
  void Map(F&& func) {
    ubyte* ptr = static_cast<ubyte*>(glMapBuffer(Target, Access));
    std::forward(func)(ptr);
    glUnmapBuffer(Target);
  }

  template <GLenum Access, typename F,
            typename = std::enable_if_t<Access == GL_READ_ONLY>>
  void Map(F&& func) const {
    const ubyte* ptr = static_cast<ubyte*>(glMapBuffer(Target, Access));
    std::forward(func)(ptr);
    glUnmapBuffer(Target);
  }

 private:
  GLuint _id;

  template <GLenum OtherTarget>
  friend class Buffer;
};

}  // namespace over
