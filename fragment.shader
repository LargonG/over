#version 330 core

in vec3 edgeColor;

out vec4 FragColor;

uniform vec4 globColor;

void main() {
	FragColor = vec4(edgeColor, 1.0) + globColor;
}