#version 330 core

struct Material {
	sampler2D texture_diffuse0;
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_diffuse4;
	sampler2D texture_diffuse5;

	sampler2D texture_specular0;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_specular3;
	sampler2D texture_specular4;
	sampler2D texture_specular5;

	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 viewPosition;
in vec3 normal;
in vec2 texCoord;

out vec4 fragColor;

uniform Material material;
uniform DirLight light;

vec3 CalcDirLight() {
	vec3 norm = normalize(normal);
	vec3 dir = normalize(light.direction);
	vec3 viewDir = normalize(-viewPosition);
	vec3 r = reflect(dir, norm);

	float diff = max(dot(-dir, norm), 0);
	float spec = pow(max(dot(r, viewDir), 0), material.shininess);

	vec3 ambient = light.ambient * texture(material.texture_diffuse0, texCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse0, texCoord).rgb;
	vec3 specular = light.specular * spec * texture(material.texture_specular0, texCoord).rgb;

	return ambient + diffuse + specular;
}

void main() {
	fragColor = vec4(CalcDirLight(), 1.0);
}