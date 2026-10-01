#include <exception>
#include <memory>
#include <optional>

#include <fmt/core.h>
#include <fmt/format.h>

#include <owlet/3d/shape.h>
#include <owlet/debug/log.h>
#include <owlet/engine/core.h>
#include <owlet/gl/core.h>
#include <owlet/gl/targets/index_buffer.h>
#include <owlet/os/core.h>
#include <owlet/types.h>

namespace {
using namespace owlet;

struct SimpleApp : engine::App<SimpleApp> {
    SimpleApp() : engine::App<SimpleApp>() {
        CreateWindow(
            {
                .width = 1980,
                .height = 980,
                .title = "Test app",
                .gl = std::make_optional(gl::Version{
                    .major = 4,
                    .minor = 6,
                }),
            },
            gl::Settings{
                .default_buffer_allocator = std::make_unique<gl::SimpleBufferAllocator>(),
                .default_vertex_array_allocator = std::make_unique<gl::SimpleVertexArrayAllocator>(),
                .default_vertex_shader_allocator = std::make_unique<gl::SimpleShaderAllocator>(GL_VERTEX_SHADER),
                .default_fragment_shader_allocator = std::make_unique<gl::SimpleShaderAllocator>(GL_FRAGMENT_SHADER),
                .default_program_allocator = std::make_unique<gl::SimpleProgramAllocator>(),
            });

        auto shape = d3::Quad();

        m_vbuffer = std::make_unique<gl::Buffer>(MainWindow().GL());
        m_ibuffer = std::make_unique<gl::Buffer>(MainWindow().GL());
        usize v_in_bytes = shape.VerticesInBytes();
        usize i_in_bytes = shape.ElementsInBytes();

        m_vbuffer->Alloc(v_in_bytes, shape.vertices.data(), gl::Buffer::Usage::StaticDraw);
        m_ibuffer->Alloc(i_in_bytes, shape.elements.data(), gl::Buffer::Usage::StaticRead);

        m_obj = std::make_unique<gl::VertexArray>(MainWindow().GL());
        m_obj->Format(0, GL_FLOAT, 3, 0);
        m_obj->Attach(0, *m_vbuffer.get(), 0, sizeof(glm::vec3));
        m_obj->BindFormats(0, {0});
        m_obj->Enable(0);
        m_obj->AttachIndex(gl::IndexBuffer(*m_ibuffer.get()));

        m_vertex_shader = std::make_unique<gl::VertexShader>(MainWindow().GL());
        std::vector<std::string> vertex_sources = {
            "#version 330\n layout(location = 0) in vec3 in_position;\n void main() { gl_Position = vec4(in_position, "
            "1.0); }\n "};
        m_vertex_shader->Sources(vertex_sources);
        if (!m_vertex_shader->Compile()) {
            fmt::println("Error: {}", m_vertex_shader->InfoLog());
            throw std::runtime_error("vertex");
        }
        std::vector<std::string> fragment_sources = {
            "#version 330\n out vec3 o_color;\n void main() { o_color.xy = gl_FragCoord.xy / 1980.0; \n o_color.z = "
            "1.0;}\n"};
        m_fragment_shader = std::make_unique<gl::FragmentShader>(MainWindow().GL());
        m_fragment_shader->Sources(fragment_sources);
        if (!m_fragment_shader->Compile()) {
            fmt::println("Error: {}", m_fragment_shader->InfoLog());
            throw std::runtime_error("fragment");
        }
        m_program = std::make_unique<gl::Program>(MainWindow().GL());

        m_program->Attach(*m_vertex_shader.get());
        m_program->Attach(*m_fragment_shader.get());
        m_program->Link();
    }

    void Update(gl::Context* gl, float dt) {
        m_program->Use();
        m_obj->Bind();

        gl->DrawElements(GL_TRIANGLES, m_ibuffer->Size<int32>(), GL_UNSIGNED_INT, 0);

        m_obj->Unbind();
    }
    void OnError(int32 err_code, std::string_view description) { fmt::println("error: {}, {}", err_code, description); }

    void OnInput(os::Window& window, int32 key_code, int32 scan_code, int32 action, int32 mode) {
        fmt::println("Input on window: {} {} {} {}", key_code, scan_code, action, mode);

        if (key_code == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            MainWindow().SetShouldClose(true);
        }
    }

    void OnMouseButton(os::Window& w, int32 button_code, int32 button_action, int32 button_mode) {}
    void OnCursorPosition(os::Window& w, float pos_x, float pos_y) {}
    void OnCursorPositionChanged(os::Window& w, float delta_x, float delta_y) {}

    std::unique_ptr<gl::Buffer> m_vbuffer;
    std::unique_ptr<gl::Buffer> m_ibuffer;
    std::unique_ptr<gl::VertexArray> m_obj;

    std::unique_ptr<gl::VertexShader> m_vertex_shader;
    std::unique_ptr<gl::FragmentShader> m_fragment_shader;
    std::unique_ptr<gl::Program> m_program;
};
}    // namespace

namespace owlet {
void Run() {
    using namespace owlet;

    std::unique_ptr<SimpleApp> app = std::make_unique<SimpleApp>();

    app->Run();
}

WLT_DEF_EMPTY_CALLBACKS

}    // namespace owlet
