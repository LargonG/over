#version 330 core

uniform struct Textures {
    sampler2D texture_diffuse0;
} material;

in VS_OUT {
    vec2 tex_position;
} fs_in;

out vec4 fs_out_color;

void main() {
    fs_out_color = texture(material.texture_diffuse0, fs_in.tex_position.yx);


    float gamma = 2.2;
    fs_out_color.xyz = pow(fs_out_color.xyz, vec3(1.0 / gamma));
}

