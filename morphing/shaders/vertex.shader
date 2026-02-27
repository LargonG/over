#version 330 core

struct Camera {
	mat4 model;
	mat4 view;
	mat4 projection;
};

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 viewPosition;
out vec3 normal;
out vec2 texCoord;

uniform Camera camera;

void main() {
	vec4 pos = camera.view * camera.model * vec4(aPosition, 1.0);
	viewPosition = pos.xyz;
	gl_Position = camera.projection * pos;
	normal = mat3(transpose(inverse(camera.view * camera.model))) * aNormal;
	texCoord = aTexCoord;
}
