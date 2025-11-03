#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

out vec3 color;
out vec3 normal;
out vec3 fragmentPosition;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(aPosition, 1.0);
	fragmentPosition = vec3(view * model * vec4(aPosition, 1.0));
	color = aColor;
	normal = mat3(transpose(inverse(view * model))) * aNormal;
	texCoord = aTexCoord;
}