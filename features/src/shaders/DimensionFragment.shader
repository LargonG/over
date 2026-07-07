#version 330 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

out vec4 FragColor;

uniform struct {
	sampler2D texture_diffuse0;

	sampler2D texture_specular0;

	float shininess;
} material;

uniform vec3 cameraPosition;
uniform samplerCube skybox;
uniform bool doReflect;

void main() {
	float ratio = 1.00 / 1.52;
	vec3 viewDirection = normalize(fPosition - cameraPosition);
	vec3 resultVector = vec3(0);
	if (doReflect) {
		resultVector = reflect(viewDirection, normalize(fNormal));
	} else {
		resultVector = refract(viewDirection, normalize(fNormal), ratio);
	}

	FragColor = vec4((texture(skybox, resultVector).rgb + texture(material.texture_diffuse0, fTexCoord).rgb) / 2.0, 1.0);
	//FragColor += texture(material.texture_diffuse0, fTexCoord);
	//FragColor = vec4(FragColor.rgb, 1.0);
}
