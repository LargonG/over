#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 tex_position;

out VS_OUT {
    vec2 tex_position;
} vs_out;

void main() {
    gl_Position = vec4(position, 1.0);
    vs_out.tex_position = tex_position;
}
