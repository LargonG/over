#include <owlet/gl/targets/buffer_target.h>

#include <owlet/gl/buffer.h>
#include <owlet/gl/context.h>

#include <owlet/gl/targets/index_buffer.h>
#include <owlet/gl/targets/uniform_buffer.h>
#include <owlet/gl/targets/vertex_buffer.h>

namespace owlet::gl {
template BufferTarget<VertexBuffer>;
template BufferTarget<IndexBuffer>;
template BufferTarget<UniformBuffer>;
}    // namespace owlet::gl
