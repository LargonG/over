#version 330 core

in VS_OUT {
    float cosine;
    vec2 g;

    vec3 rayleigh;
    vec3 mie;
} fs_in;

out vec4 fs_out_color;

float PhaseFunction(float cosine, float g) {
    float g2 = g * g;
    return (3 * (1 - g2)) / (2 * (2 + g2)) * (1 + cosine * cosine) / pow(1 + g2 - 2 * g * cosine, 3.0 / 2.0);
}

void main() {
    vec3 rayleigh = fs_in.rayleigh * PhaseFunction(fs_in.cosine, fs_in.g.x);
    vec3 mie = fs_in.mie * PhaseFunction(fs_in.cosine, fs_in.g.y);
    
    fs_out_color.rgb = rayleigh + mie;
    fs_out_color.a = fs_out_color.b;
}
