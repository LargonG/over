#version 330 core

layout (location = 0) in vec3 aPosition;

uniform vec3 model;
uniform vec3 view;
uniform vec3 projection;

void main() {
	gl_Position = projection * view * model * vec4(aPosition, 1.0);
}