#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 color;
in vec3 normal;
in vec3 fragmentPosition;
in vec2 texCoord;

out vec4 FragColor;

uniform vec3 viewPosition;
uniform Material material;

uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform float timePassed;

vec3 CalcDirLight(DirLight light);
vec3 CalcPointLight(PointLight light);
vec3 CalcSpotLight(SpotLight light);

void main() {
	vec3 result = vec3(0);
	result += CalcDirLight(dirLight);
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalcPointLight(pointLights[i]);
	}
	result += CalcSpotLight(spotLight);

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light) {
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(-light.direction);
	vec3 viewDir = normalize(-fragmentPosition); // view basis
	vec3 reflectDir = reflect(-lightDir, norm);

	float diff = max(dot(norm, lightDir), 0.0);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light) {
	vec3 norm = normalize(normal);
	vec3 lightDiff = light.position - fragmentPosition;
	vec3 lightDir = normalize(lightDiff);
	vec3 viewDir = normalize(-fragmentPosition); // view basis
	vec3 reflectDir = reflect(-lightDir, norm);

	float distance = length(lightDiff);
	float attenuation = min(1.0, 1.0 / (light.constant + light.linear * distance +
		light.quadratic * distance * distance));
	float diff = max(dot(norm, lightDir), 0.0);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * attenuation * texture(material.diffuse, texCoord).rgb;
	vec3 diffuse = light.diffuse * diff * attenuation * texture(material.diffuse, texCoord).rgb;
	vec3 specular = light.specular * spec * attenuation * texture(material.specular, texCoord).rgb;

	return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light) {
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragmentPosition);
	vec3 viewDir = normalize(-fragmentPosition); // view basis
	vec3 reflectDir = reflect(-lightDir, norm);

	float theta = dot(normalize(light.direction), -lightDir);
	float epsilon = light.outerCutOff - light.cutOff;
	float intensity = smoothstep(0.0, 1.0, (light.outerCutOff - theta) / epsilon);

	float diff = max(dot(lightDir, norm), 0.0);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;
	vec3 diffuse = light.diffuse * diff * intensity * texture(material.diffuse, texCoord).rgb;
	vec3 specular = light.specular * spec * intensity * texture(material.specular, texCoord).rgb;

	return ambient + diffuse + specular;
}