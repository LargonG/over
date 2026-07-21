#version 330 core

out vec4 FragColor;

in vec3 fTexCoords;

uniform samplerCube skybox;

void main() {
	float gamma = 2.2;
	FragColor = texture(skybox, fTexCoords);
	FragColor.rgb = pow(FragColor.rgb, vec3(gamma));
}
