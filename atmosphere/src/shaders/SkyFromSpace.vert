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

layout (location = 0) in vec3 in_position;


out VS_OUT {
    vec3 direction;

    vec3 rayleigh;
    vec3 mie;
} vs_out;

vec3 RayCast(vec3 start, vec3 direction, float radius) {
    float angle = dot(start, direction);
    float angle2 = angle * angle;
    float start_length2 = dot(start, start);
    float radius2 = radius * radius;

    float F2 = angle2 - (start_length2 - radius2);
    float near = -angle - sqrt(max(0, F2));
    float far = -angle + sqrt(max(0, F2));
    
    return vec3(F2, near, far);

}

bool RayCastIntersect(vec3 cst) {
    return cst.x >= 0 && cst.z >= 0;
}

float RayCastNear(vec3 cst) {
    return cst.y;
}

float RayCastFar(vec3 cst) {
    return cst.z;
}

vec2 LookupPosition(float h, float cosine) {
    return vec2(h, cosine / 2.0 + 0.5);
}

vec3 InScattering(int samples, vec3 near, vec3 far, float h0,
                  vec3 light_value, vec3 light_direction,
                  vec3 kernel, mat2x3 kernels,
                  float planet_radius, float sky_radius) {
    float sky_thickness = sky_radius - planet_radius;

    vec3 ray = far - near;

    float ds = length(ray) / (samples * sky_thickness);
    float near_angle = dot(ray, near) / (length(ray) * length(near));
    vec2 near_depth = texture(lookup, LookupPosition(1, near_angle)).rg;

    vec3 result = vec3(0);
    for (int i = 0; i < samples; i++) {
        float percent = (i + 0.5) / samples;
        
        vec3 position = near + ray * percent;

        float h = (length(position) - planet_radius) / sky_thickness;

        // cosines in [-1, 1]
        float light_angle = dot(light_direction, position) / (length(position) * length(light_direction));
        float camera_angle = dot(ray, position) / (length(ray) * length(position));

        float density = exp(-h / h0);

        vec2 light_depth = texture(lookup, LookupPosition(h, light_angle)).rg;
        vec2 sample_depth = texture(lookup, LookupPosition(h, camera_angle)).rg;

        // scattering from rayleigh & mie at the same time
        vec2 scatter = (near_depth - sample_depth) + light_depth;
        
        vec3 k_scatter = kernels * scatter;

        vec3 attenute = exp(-4.0 * args.pi * k_scatter);
        result += ds * density * attenute;
    }

    result = result * light_value * kernel;

    return result;
}

void main() {
    float planet_radius = args.radius.x;
    float sky_radius = args.radius.y;

    // Calculate real position of vertex
    vec4 position = mvp.model * vec4(in_position, 1.0);
    gl_Position = mvp.projection * mvp.view * position;

    vec3 direction = normalize(position.xyz - camera.position);

    // Intersection must be
    vec3 sky_cast = RayCast(camera.position, direction, sky_radius);
    vec3 planet_cast = RayCast(camera.position, direction, planet_radius);
    
    float t = max(RayCastNear(sky_cast), RayCastFar(planet_cast));
    vec3 near = camera.position + float(t > 0) * direction * t;
    vec3 far = camera.position + direction * RayCastFar(sky_cast);

    vec3 to_light = normalize(-sun.direction);
    float light_angle = dot(to_light, near) / length(near);
    
    vec3 rayleigh_kernel = args.rayleigh_kernel.xyz;
    vec3 mie_kernel = args.mie_kernel.xyz;
    mat2x3 kernels = mat2x3(rayleigh_kernel, mie_kernel);

    vs_out.direction = direction;
    
    vs_out.rayleigh = InScattering(args.samples.x, near, far, args.h0.x, sun.light, to_light, rayleigh_kernel, kernels, planet_radius, sky_radius);
    vs_out.mie = InScattering(args.samples.y, near, far, args.h0.y, sun.light, to_light, mie_kernel, kernels, planet_radius, sky_radius);
}
