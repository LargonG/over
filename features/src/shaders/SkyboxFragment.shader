#version 330 core

out vec4 FragColor;

in vec3 fTexCoords;

uniform samplerCube skybox;

void main() {
	FragColor = texture(skybox, fTexCoords);
}
