#version 330 core

out vec4 FragColor;

in vec2 fTexCoords;

uniform sampler2D screenTexture;

void main() {
	FragColor = texture(screenTexture, fTexCoords);
}
