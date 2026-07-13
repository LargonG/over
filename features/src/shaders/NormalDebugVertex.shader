#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

out VS_OUT {
	vec3 position;
	vec3 normal;
} vs_out;

layout (std140) uniform Camera {
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

void main() {
	vec4 position = model * vec4(vPosition, 1.0);

	vs_out.position = position.xyz;
	vs_out.normal = mat3(transpose(inverse(model))) * vNormal;

	gl_Position = projection * view * position;
}
