#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 position;
	vec3 normal;
} gs_in[];

layout (std140) uniform Camera {
	mat4 projection;
	mat4 view;
};

uniform float magnitude;

vec4 vp(vec3 position, float movable) {
	return projection * view * vec4(position, movable);
}

void AddLine(int i) {
	gl_Position = vp(gs_in[i].position, 1.0);
	EmitVertex();

	gl_Position = vp(gs_in[i].position + gs_in[i].normal * magnitude, 1.0);
	EmitVertex();
	EndPrimitive();
}

void main() {
	for (int i = 0; i < 3; i++) {
		AddLine(i);
	}
}
