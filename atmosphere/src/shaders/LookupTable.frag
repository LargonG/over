#version 330 core

layout (std140) uniform LookupTableArguments {
    ivec2 samples;
    // framebuffer size == viewport size
    ivec2 size;
    // base_height should be already normalized
    vec2 h0;
    // radius.x = planet, radius.y = atmosphere
    float scale_factor;
} args;

out vec2 fs_out_color;

vec2 rotate(in float cosine, in float sinuse, in vec2 v) {
    mat2 rot = mat2( vec2(cosine, sinuse), vec2(-sinuse, cosine));
    return rot * v;
}

vec2 rotate( in float rad, vec2 v)
{
    float cosine = cos(rad);
    float sinuse = sin(rad);
    return rotate(cosine, sinuse, v);
}

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
   
float depth_loop(
in int samples,
in float h0,
in vec3 start,
in vec3 end,
in float thickness)
{
    vec3 ray = end - start;
    float ds = length(ray) / float(samples);
      
    float result = 0.0;
    for (int i = 0; i < samples; i++)
    {
        float percent = float(i) * 1.0 / float(samples);
        vec3 pos = start + ray * percent; // absolute coords
         
          
        // pos_h in percents of atm_thickness
        float pos_h = (length(pos) - 1.0) / thickness;
         
        float intp = max(0.0, pos_h); 
          
        float depth = exp(-intp / h0);
         
        result += depth * ds;
    }
       
    return result;
}

void optical(in float h, in float cosine, in float scale_factor, in float thickness,
            out vec3 start, out vec3 dir, out vec3 planet_cast, out vec3 atm_cast)
{   
    start = vec3(0, 1.0 + thickness * h, 0);
      
    vec2 up = vec2(0.0, 1.0);
    float cosine2 = cosine * cosine;
    float sinuse = sqrt(1.0 - cosine2);
       
    dir = vec3(rotate(cosine, sinuse, up), 0);
      
    planet_cast = ray_cast(start, dir, 1.0);
    atm_cast = ray_cast(start, dir, scale_factor * scale_factor);
}

// TODO: Should be precomputed
float optical_depth_table(
    in int samples,
    in float h,
    in float h0,
    in float cosine,
    in float scale_factor,
    in float p)
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
      
    vec3 start, dir, planet_cast, atm_cast;
    optical(h, cosine, scale_factor, thickness,
            // out
            start, dir, planet_cast, atm_cast);
       
    float k = far(atm_cast);
    float ground = near(planet_cast);
      
    vec3 end = start + k * dir * p;
    vec3 planet = start + ground * dir * p;
      
    return depth_loop(samples, h0, start, end, thickness); 
        //intersect(planet_cast, epsIn, epsFar) * depth_loop(samples, h0, planet, end, thickness);
}


// Interpretations
float as_table(int samples, vec2 st, float h0, float scale_factor)
{
    // st.x in [0, 1]
    // want: [-1, 1]
    // st.x * 2 - 1 in [-1, 1]
	float h = st.x;
    float cos = (st.y * 2.0) - 1.0;
    
    float res = optical_depth_table(samples, h, h0, cos, scale_factor, 1.0);
    return res;
}

float as_paint(int samples, vec2 st, vec2 center, float h, float h0, float scale_factor)
{
    vec2 up = vec2(0.0, 1.0);
    vec2 dir = normalize(st - center);
    
    float cosine = dot(dir, up);
    
    float res = optical_depth_table(samples, h, h0, cosine, scale_factor, 1.0);
    
    return res;
}

float as_point_paint(int samples, vec2 st, vec2 center, float h, float h0, float scale_factor, float max_dist)
{
    vec2 up = vec2(0.0, 1.0);
    vec2 dir = normalize(st - center);
    
    float cosine = dot(dir, up);
    
    float res = optical_depth_table(samples, h, h0, cosine, scale_factor, length(st - center) / max_dist);
    res *= smoothstep(-0.1, 0.1, length(st - center) / max_dist);
    
    return res;
}


void main() {
    vec2 st = gl_FragCoord.xy * vec2(1.0) / args.size.xy;
    st.x *= args.size.x * 1.0 / args.size.y;

    ivec2 samples = args.samples.xy;
    vec2 h0 = args.h0;
    float scale_factor = args.scale_factor;

    vec2 color = vec2(
        as_table(samples.x, st, h0.x, scale_factor),
        as_table(samples.y, st, h0.y, scale_factor));

    fs_out_color = color;
}
