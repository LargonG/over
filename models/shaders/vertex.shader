#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 viewPosition;
out vec3 normal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	vec4 pos = view * model * vec4(aPosition, 1.0);
	viewPosition = pos.xyz;
	gl_Position = projection * pos;
	normal = mat3(transpose(inverse(view * model))) * aNormal;
	texCoord = aTexCoord;
}