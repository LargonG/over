#version 330 core

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Camera {
	mat4 model;
	mat4 view;
	mat4 projection;
};

struct Material {
	sampler2D texture_diffuse0;

	sampler2D texture_specular0;

	float shininess;
};

struct DirLight {
	vec3 direction;

	Light light;
};

struct SpotLight {
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;

	Light light;
};

in vec3 viewPosition;
in vec3 normal;
in vec2 texCoord;

out vec4 fragColor;

uniform Material material;
uniform DirLight directionalLight;
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light) {
	vec3 norm = normalize(normal);
	vec3 dir = normalize(-light.direction);
	vec3 viewDir = normalize(-viewPosition);
	vec3 r = reflect(-dir, norm);

	float diff = max(dot(dir, norm), 0);
	float spec = pow(max(dot(r, viewDir), 0), material.shininess);

	vec3 ambient = light.light.ambient; // * texture(material.texture_diffuse0, texCoord).rgb;
	vec3 diffuse = light.light.diffuse * diff; // * texture(material.texture_diffuse0, texCoord).rgb;
	vec3 specular = light.light.specular * spec; // * texture(material.texture_specular0, texCoord).rgb;

	return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light) {
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - viewPosition);
	vec3 viewDir = normalize(-viewPosition); // view basis
	vec3 reflectDir = reflect(-lightDir, norm);

	float theta = dot(normalize(light.direction), -lightDir);
	float epsilon = light.outerCutOff - light.cutOff;
	float intensity = smoothstep(0.0, 1.0, (light.outerCutOff - theta) / epsilon);

	float diff = max(dot(lightDir, norm), 0.0);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.light.ambient;// * texture(material.diffuse, texCoord).rgb;
	vec3 diffuse = light.light.diffuse * diff * intensity;// * texture(material.diffuse, texCoord).rgb;
	vec3 specular = light.light.specular * spec * intensity;// * texture(material.specular, texCoord).rgb;

	return ambient + diffuse + specular;
}

void main() {
	fragColor = vec4(CalcDirLight(directionalLight) + CalcSpotLight(spotLight), 1.0);
}
