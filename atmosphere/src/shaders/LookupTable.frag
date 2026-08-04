#version 330 core

layout (std140) uniform LookupTableArguments {
    // framebuffer size == viewport size
    ivec2 size;
    // base_height should be already normalized
    vec2 h0;
    // radius.x = planet, radius.y = atmosphere
    vec2 radius;
    ivec2 samples;
} args;

out vec2 fs_out_color;

vec2 GetPosition(vec2 pos) {
    return pos / args.size;
}

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


float OpticalDepth(int samples, float x, float y, float h0, float planet_radius, float sky_radius) {
    // y in [0, 1] => (y - 0.5) * 2 in [-1, 1]
    // (y - 0.5) * 2 == cos(angle)
    // => cos(angle) / 2 + 0.5 = y
    // float angle = acos((y - 0.5) * 2);

    float sky_thickness = sky_radius - planet_radius;

    // calculate direction
    float cosine = (y - 0.5) * 2;
    float sinuse = sqrt(1 - cosine * cosine);
    vec2 direction = normalize(mat2(cosine, sinuse, -sinuse, cosine) * vec2(0, 1));

    vec2 start = vec2(0, planet_radius + x * sky_thickness);
    
    vec3 startv3 = vec3(start, 0);
    vec3 directionv3 = vec3(direction, 0);

    vec3 planet_cast = RayCast(startv3, directionv3, planet_radius);
    vec3 sky_cast = RayCast(startv3, directionv3, sky_radius);

    bool planet_intersection = RayCastIntersect(planet_cast);

    float planet_near = RayCastNear(planet_cast);
    float sky_far = RayCastFar(sky_cast);

    float point = float(planet_intersection) * planet_near + float(!planet_intersection) * sky_far;

    vec2 end = start + direction * point;

    vec2 ray = end - start;
    float ds = length(ray) / (samples * sky_thickness);

    float result = 0;
    for (int i = 0; i < samples; i++) {
        float percent = (i + 0.5) / samples;
        vec2 position = start + ray * percent;

        // incorrect: h = length(position) - planet_radius, because h0 is in percent (coefficient) mode!
        // correct:
        float h = (length(position) - planet_radius) / sky_thickness;

        result += exp(- h / h0);
    }

    result *= ds;

    return result;
}

void main() {
    vec2 pos = GetPosition(gl_FragCoord.xy);

    float planet_radius = args.radius.x;
    float sky_radius = args.radius.y;

    fs_out_color = vec2(
        OpticalDepth(args.samples.x, pos.x, pos.y, args.h0.x, planet_radius, sky_radius),
        OpticalDepth(args.samples.y, pos.x, pos.y, args.h0.y, planet_radius, sky_radius)
    );
}
