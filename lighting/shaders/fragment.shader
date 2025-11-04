#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 position;

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
uniform Light light;

void main() {
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragmentPosition);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));

	vec3 viewDir = normalize(-fragmentPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));

	vec3 emission = texture(material.emission, texCoord).rgb;

	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result, 1.0);
}