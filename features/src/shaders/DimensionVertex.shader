#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} vs_out;

layout (std140) uniform Camera {
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(vPosition, 1.0);

	vs_out.position = vec3(model * vec4(vPosition, 1.0));
	vs_out.normal = mat3(transpose(inverse(model))) * vNormal;
	vs_out.texCoord = vTexCoord;
}
