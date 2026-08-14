#version 330 core

#define PI 3.14

layout (std140) uniform World {
    mat4 projection;
    mat4 view;
    mat4 model;
} mvp;

layout (std140) uniform Camera {
    vec4 position;
} camera;

layout (std140) uniform Light {
    vec4 direction;
    vec4 light;
} sun;

layout (std140) uniform Args {
    ivec2 samples;
    vec2 h0;
    vec2 g;
    float scale_factor;
} u_args;

layout (std140) uniform Kernels {
    vec4 rayleigh;
    vec4 mie;
} u_kernels;

uniform sampler2D lookup;

layout (location = 0) in vec3 in_position;


out VS_OUT {
    vec3 direction;

    vec3 rayleigh;
    vec3 mie;
} vs_out;


vec3 ray_cast( in vec3 pos, in vec3 ray, in float radius2)
{
    // Invariants:
    // vec3(0) == sphere_center
    // length(ray) == 1
    // radius2 = sphere radius ^ 2
      
    float angle = dot(pos, ray);
    float angle2 = angle * angle;
    float pos_length2 = dot(pos, pos);
      
    float D2 = angle2 + radius2 - pos_length2;
    float D = sqrt(max(0.0, D2));
    float near = -angle - D;
    float far = -angle + D;
      
    // note: returns D2, not D
    return vec3(D2, near, far);
}

float intersect(in vec3 raycast, in float epsIn, in float epsFar)
{
    return smoothstep(-epsIn, epsIn, raycast.x) * smoothstep(0.0, epsFar, raycast.z);
}
   
float near(in vec3 raycast)
{
    return raycast.y;
}
   
float far(in vec3 raycast)
{
    return raycast.z;
}

vec2 lookup_position(float h, float cosine) {
    return vec2(h, (cosine + 1.0)/ 2.0);
}

float get_h(vec3 pos, float thickness)
{
    return (length(pos) - 1.0) / thickness;
}

vec3 scatter_loop(
in int samples,
in float h0,
in vec3 start,
in vec3 end,
in vec3 light_dir,
in vec3 light_val,
in mat2x3 kernels,
in float thickness)
{
    const float epsIn = 0.212;
    const float epsFar = 1.125;
    vec3 ray = end - start;
    float ds = length(ray) / float(samples);

    float start_h = get_h(start, thickness);
    float start_angle = dot(ray, start) / (length(ray) * length(start));
    vec2 start_depth = texture(lookup, lookup_position(start_h, start_angle)).rg;
      
    vec3 result = vec3(0.0);
    for (int i = 0; i < samples; i++)
    {
        float percent = float(i) * 1.0 / float(samples);
        vec3 pos = start + ray * percent; // absolute coords
         
          
        // pos_h in percents of atm_thickness
        float pos_h = get_h(pos, thickness);
         
        float intp = max(0.0, pos_h); 
        
        float depth = exp(-intp / h0);
        
        float light_cos = dot(pos, light_dir) / (length(pos) * length(light_dir));
        float camera_cos = dot(ray, pos) / (length(ray) * length(pos));

        vec3 planet_cast = ray_cast(pos, light_dir, 1.0);
        vec3 planet_pos = pos + light_dir * near(planet_cast);

        float lightp = get_h(planet_pos, thickness);

        vec2 light_depth =  texture(lookup, lookup_position(intp, light_cos)).rg;
        vec2 sample_depth = texture(lookup, lookup_position(intp, camera_cos)).rg;
        vec2 light_ground_depth = intersect(planet_cast, epsIn, epsFar) *
        texture(lookup, lookup_position(lightp, light_cos)).rg;

        vec2 scatter = start_depth + ((light_depth) - sample_depth);

        vec3 k_scatter = kernels * scatter;

        vec3 attenute = exp(-4.0 * PI * k_scatter);

        result += attenute * depth * ds;
    }
       
    return result;
}

vec3 in_scattering(
    in int samples,
    in vec3 start,
    in vec3 end,
    in vec3 light_dir,
    in vec3 light_val,
    in vec3 kernel,
    in mat2x3 kernels,
    in float h0,
    in float scale_factor)
{
    // Invariants:
    // samples - integral precision
    // h, h0 - in atm_thickness percents; (h in [0, 1])
    // cos_angle in [-1, 1] (it's cos...)
    // angle in [180, 0] (inverted)
       
    // planet_radius = 1.f;

    const float epsIn = 0.212;
    const float epsFar = 1.125;
   
    float thickness = scale_factor - 1.0;
    vec3 dir = normalize(end - start);

    vec3 all_result = scatter_loop(samples, h0, start, end, light_dir, light_val, kernels, thickness);

    vec3 planet_cast = ray_cast(start, dir, 1.0);

    vec3 planet_start = start + near(planet_cast) * dir;

    vec3 planet_result = intersect(planet_cast, epsIn, epsFar) *
    scatter_loop(samples, h0, planet_start, end, light_dir, light_val, kernels, thickness);

    return (all_result - planet_result) * light_val * kernel;
}

void main() {
    // camera_position should be in RELATIVE coordinates
    vec3 camera_position = camera.position.xyz;

    vec3 direction = normalize(in_position * u_args.scale_factor - camera_position);

    // Intersection must be
    vec3 sky_cast = ray_cast(camera_position, direction, u_args.scale_factor * u_args.scale_factor);
    
    // camera could be inside atmosphere
    vec3 near = camera_position + direction * near(sky_cast) * float(near(sky_cast) > 0);
    vec3 far = camera_position + direction * far(sky_cast);

    vec3 to_light = normalize(-sun.direction.xyz);
    float light_angle = dot(to_light, near) / length(near);
    
    mat2x3 kernels = mat2x3(u_kernels.rayleigh.xyz, u_kernels.mie.xyz);

    vs_out.direction = direction;
    
    vs_out.rayleigh = in_scattering(u_args.samples.x, near, far,
                                    to_light, sun.light.xyz,
                                    u_kernels.rayleigh.xyz, kernels,
                                    u_args.h0.x, u_args.scale_factor);
    vs_out.mie = in_scattering( u_args.samples.y, near, far,
                                to_light, sun.light.xyz,
                                u_kernels.mie.xyz, kernels,
                                u_args.h0.y, u_args.scale_factor);
                                

    gl_Position = mvp.projection * mvp.view * mvp.model * vec4(in_position, 1.0);
}
