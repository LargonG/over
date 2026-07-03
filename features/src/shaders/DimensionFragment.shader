#version 330 core

in vec2 fTexCoord;

out vec4 FragColor;

uniform struct {
	sampler2D texture_diffuse0;

	sampler2D texture_specular0;

	float shininess;
} material;

uniform samplerCube cubemap;

void main() {
	FragColor = texture(material.texture_diffuse0, fTexCoord);
}
