export module owlet.gl:parameters;

export namespace lowl::gl {

class BufferAllocator;

struct ContextParameters {
    // To allocate context we need parameters
    // But to allocate parameters we need initialized context
    // So we need (variants)
    // 1) Create wrapper class for ctor function storage
    // 2) Make ctors of all parameters default, and pass context as function call
    // 3) Metaprogramming or define magic
    // 4) Or... pass lambda function? As argument take "constructed" context

    BufferAllocator* default_buffer_allocator;
};
}    // namespace lowl::gl
