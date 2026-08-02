#version 330 core

layout (std140) uniform LookupTableArguments {
    // framebuffer size == viewport size
    ivec2 size;
    // base_height should be already normalized
    vec2 base_height;
    // radius.x = planet, radius.y = atmosphere
    vec2 radius;
    ivec2 samples;
} args;

out vec2 fs_out_color;

vec2 GetPosition(vec2 pos) {
    return pos / args.size;
}

bool RayIntersect(vec3 start, vec3 direction, float radius) {
    float angle = dot(start, direction);
    float angle2 = angle * angle;
    float start_length2 = dot(start, start);
    float radius2 = radius * radius;

    float F2 = angle2 - (start_length2 - radius2);
    float t = -angle + sqrt(max(0, F2));
    return F2 >= 0 && t >= 0;
}

float RayCastFar(vec3 start, vec3 direction, float radius) {
    float angle = dot(start, direction);
    float angle2 = angle * angle;
    float start_length2 = dot(start, start);
    float radius2 = radius * radius;

    float F2 = angle2 - (start_length2 - radius2);
    float t = -angle + sqrt(max(0, F2));
    return t;
}

float RayCastNear(vec3 start, vec3 direction, float radius) {
    float angle = dot(start, direction);
    float angle2 = angle * angle;
    float start_length2 = dot(start, start);
    float radius2 = radius * radius;

    float F2 = angle2 - (start_length2 - radius2);
    float t = -angle - sqrt(max(0, F2));
    return t;
}

//float GetOpticalDepth(float height, vec2 direction, float base_height, int samples) {
    
    
    
    /*// constants
    float planet_radius = args.radius.x;
    float atmosphere_radius = args.radius.y;
    float atmosphere_thickness = atmosphere_radius - planet_radius;
    
    float planet_radius2 = planet_radius * planet_radius;
    float atmosphere_radius2 = atmosphere_radius * atmosphere_radius;

    // start sample position
    vec2 init_position = vec2(0.0, planet_radius + height * atmosphere_thickness);

    // calculate end sample position

    bool intersect_planet = RayIntersect(vec3(init_position, 0), vec3(direction, 0), planet_radius);
    bool intersect_sky = RayIntersect(vec3(init_position, 0), vec3(direction, 0), atmosphere_radius);

    float planet_cast = RayCastNear(vec3(init_position, 0), vec3(direction, 0), planet_radius);
    float sky_cast = RayCastFar(vec3(init_position, 0), vec3(direction, 0), atmosphere_radius);

    vec2 planet_point = float(intersect_planet) *
        (init_position + direction * planet_cast);
    vec2 atmosphere_point = float(!intersect_planet && intersect_sky) *
        (init_position + direction * sky_cast);

    // end sample position
    vec2 end_position = planet_point + atmosphere_point;

    // sample step
    float ds = length(end_position - init_position) / samples;

    float accum = 0;
    for (int i = 0; i < samples; i++) {
        vec2 position = init_position + ds * direction * (i + 0.5);

        float cur_height = length(position) - planet_radius;
        accum += ds * exp(- cur_height / base_height);
    }

    return accum;
    */
//}

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
    bool planet_intersection = RayIntersect(startv3, directionv3, planet_radius);
    bool sky_intersection = RayIntersect(startv3, directionv3, sky_radius);

    float planet_near = RayCastNear(startv3, directionv3, planet_radius);
    float sky_far = RayCastFar(startv3, directionv3, sky_radius);

    float point = float(planet_intersection) * planet_near + float(!planet_intersection && sky_intersection) * sky_far;

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
        OpticalDepth(args.samples.x, pos.x, pos.y, args.base_height.x, planet_radius, sky_radius),
        OpticalDepth(args.samples.y, pos.x, pos.y, args.base_height.y, planet_radius, sky_radius)
    );

    //    float sinuse = sin(angle);
//    float cosine = cos(angle);
//    vec2 direction = normalize(mat2(cosine, -sinuse, sinuse, cosine) * vec2(0.0, 1.0));
//    fs_out_color = vec2(
//        GetOpticalDepth(height, direction, args.base_height.x, args.samples.x),
//        GetOpticalDepth(height, direction, args.base_height.y, args.samples.y)
//    );
}
