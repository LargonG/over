#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} gs_in[];

out VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} gs_out;

layout (std140) uniform Camera {
	mat4 projection;
	mat4 view;
};

uniform float time;

vec4 vp(vec3 position, float movable) {
	return projection * view * vec4(position, movable);
}

vec3 Explode(float dist, vec3 position, vec3 direction) {
	return position + dist * (sin(time) / 1.2) * direction;
}

void main() {
	float magnitude = 20;
	vec3 avg = vec3(0);
	for (int i = 0; i < 3; i++) {
		avg += gs_in[i].normal;
	}
	avg /= 3.0;
	for (int i = 0; i < 3; i++) {
		gs_out.position = Explode(magnitude, gs_in[i].position, normalize(avg));
		gl_Position = vp(gs_out.position, 1.0);
		
		gs_out.normal = gs_in[i].normal;
		gs_out.texCoord = gs_in[i].texCoord;
		
		EmitVertex();
	}
	EndPrimitive();
}
