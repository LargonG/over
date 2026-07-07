#version 330 core

#define KERNEL_SIZE 9

out vec4 FragColor;

in vec2 fTexCoord;

uniform struct {
	sampler2D texture_diffuse0;

	bool inverted;
} material;

const float offset = 1.0 / 300.0;

void main() {
	vec2 offsets[KERNEL_SIZE] = vec2[](
		vec2(-offset, offset),
		vec2(0.0, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0),
		vec2(0.0, 0.0),
		vec2(offset, 0.0),
		vec2(-offset, -offset),
		vec2(0.0, -offset),
		vec2(offset, -offset)
	);
	
	float kernel[KERNEL_SIZE] = float[](
		-1.0, 3.0, -1.0,
		3.0, -8.0, 3.0,
		-1.0, 3.0, -1.0
	);

	/*
	float kernel[KERNEL_SIZE] = float[](
		1.0, 2.0, 1.0,
		2.0, 4.0, 2.0,
		1.0, 2.0, 1.0
	);*/

		FragColor = texture(material.texture_diffuse0, fTexCoord);
	if (!material.inverted) {
	} else {
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
	
	// FragColor = texture(material.texture_diffuse0, fTexCoord);
	// if (material.inverted) {
	// 	FragColor = vec4(vec3(1.0 - FragColor), 1.0);
	// 	float avg = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
	// 	FragColor = vec4(avg, avg, avg, 1.0);
	// 	FragColor = vec4(FragColor.b, FragColor.r, FragColor.g, 1.0);
	// 	float rw = 0.2126;
	// 	float gw = 0.7152;
	// 	float bw = 0.0722;

	// 	float avg = rw * FragColor.r + gw * FragColor.g + bw * FragColor.b;
	// 	FragColor = vec4(avg, avg, avg, 1.0);	
	// }
}
