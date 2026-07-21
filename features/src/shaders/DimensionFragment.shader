#version 330 core

in VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} fs_in;

out vec4 FragColor;

uniform struct {
	sampler2D texture_diffuse0;

	sampler2D texture_specular0;

	float shininess;
} material;

uniform vec3 cameraPosition;
uniform samplerCube skybox;
uniform bool doReflect;

vec4 correct(vec4 color, float gamma) {
	return vec4(pow(color.rgb, vec3(gamma)), color.a);
}

void main() {
	float ratio = 1.00 / 1.52;
	vec3 viewDirection = normalize(fs_in.position - cameraPosition);
	vec3 resultVector = vec3(0);

	resultVector = float(doReflect) * reflect(viewDirection, normalize(fs_in.normal)) +
				float(!doReflect) * refract(viewDirection, normalize(fs_in.normal), ratio);

	float gamma = 2.2;
	vec4 skyboxColor = correct(texture(skybox, resultVector), gamma);
	vec4 modelColor = correct(texture(material.texture_diffuse0, fs_in.texCoord), gamma);

	FragColor = vec4((skyboxColor + modelColor).rgb / 2.0, 1.0);
	//FragColor += texture(material.texture_diffuse0, fTexCoord);
	//FragColor = vec4(FragColor.rgb, 1.0);
}
