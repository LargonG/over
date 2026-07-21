#version 330 core

#define KERNEL_SIZE 9

out vec4 FragColor;

in vec2 fTexCoord;

uniform struct {
	sampler2D texture_diffuse0;

	bool inverted;
} material;

layout (std140) uniform Screen {
	ivec2 size;
};

void main() {
	float offset_x = 1.0 / size.x * 2.0;
	float offset_y = 1.0 / size.y * 2.0;

	vec2 offsets[KERNEL_SIZE] = vec2[](
		vec2(-offset_x, offset_y),
		vec2(0.0, offset_y),
		vec2(offset_x, offset_y),
		vec2(-offset_x, 0.0),
		vec2(0.0, 0.0),
		vec2(offset_x, 0.0),
		vec2(-offset_x, -offset_y),
		vec2(0.0, -offset_y),
		vec2(offset_x, -offset_y)
	);
	
	float kernel[KERNEL_SIZE] = float[](
		1.0, 1.0, 1.0,
		1.0, -8.0, 1.0,
		1.0, 1.0, 1.0
	);

	FragColor = texture(material.texture_diffuse0, fTexCoord);
	if (material.inverted) {
		vec3 samples[KERNEL_SIZE];
		for (int i = 0; i < KERNEL_SIZE; i++) {
			samples[i] = vec3(texture(material.texture_diffuse0, fTexCoord + offsets[i]));
		}

		vec3 result = vec3(0.0);
		for (int i = 0; i < KERNEL_SIZE; i++) {
			result += kernel[i] * samples[i];
		}
		FragColor = vec4(result, 1.0);
	}

	float gamma = 2.2;
	FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));
}
