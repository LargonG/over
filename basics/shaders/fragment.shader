#version 330 core

in vec3 color;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 globColor;

void main() {
	// FragColor = vec4(color, 1.0) + globColor;
	// FragColor = texture(texture1, texCoord) + vec4(color, 1.0) + globColor;
	// FragColor = mix(texture(texture1, texCoord), texture(texture2,
	// 	2 * vec2(1.0 - texCoord.x, texCoord.y)), 0.2);
	FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2) +
				vec4(color, 1.0) + globColor;
}