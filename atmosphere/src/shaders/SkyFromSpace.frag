#version 330 core

layout (std140) uniform World {
    mat4 projection;
    mat4 view;
    mat4 model;
} mvp;

layout (std140) uniform Camera {
    vec3 position;
} camera;

layout (std140) uniform Light {
    vec3 direction;
    vec3 light;
} sun;

layout (std140) uniform Args {
    vec4 rayleigh_kernel;
    vec4 mie_kernel;
    vec2 radius;
    vec2 h0;
    vec2 g;
    ivec2 samples;
    float pi;
} args;

uniform sampler2D lookup;

in VS_OUT {
    vec3 direction;

    vec3 rayleigh;
    vec3 mie;
} fs_in;

out vec4 fs_out_color;

float PhaseFunction(float cosine, float g) {
    float cosine2 = cosine * cosine;
    float g2 = g * g;
    
    float first = 3.0 / 2.0 * (1.0 - g2) / (2.0 + g2);
    float second = pow((1.0 + cosine2) / (1.0 + g2 - 2.0 * g * cosine2), 3.0 / 2.0);

    return first * second;
}

void main() {
    vec3 to_light = normalize(-sun.direction);
    float cosine = dot(to_light, fs_in.direction) / length(fs_in.direction);

    vec3 rayleigh = fs_in.rayleigh * PhaseFunction(cosine, args.g.x);
    vec3 mie = fs_in.mie * PhaseFunction(cosine, args.g.y);
    
    fs_out_color.rgb = rayleigh + mie;
    fs_out_color.a = fs_out_color.b;

    fs_out_color = vec4(1.0) - exp(-vec4(8.f) * fs_out_color);
}
