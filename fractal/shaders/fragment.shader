#version 330 core

in vec3 color;
in vec3 position;

out vec4 FragColor;

uniform vec2 offset;
uniform int k;
uniform int p;
uniform float gamma;

vec2 complexMul(vec2 a, vec2  b) {
	return vec2(
		a.x * b.x - a.y * b.y,
		a.x * b.y + a.y * b.x
	);
}

vec2 complexPow(vec2 z, int p) {
	// logariphmic solution works slower for more common cases, like 2, 3, or 5
	vec2 result = vec2(1, 0);
	for (int i = 0; i < p; i++) {
		result = complexMul(result, z);
	}
	return result;
}

void main() {
	vec2 z0 = (position.xy + offset);

	int iter = k;
	vec2 z = vec2(0, 0);
	for (iter = 0; iter < k; iter++) {
		z = complexPow(z, p) + z0;
		if (z.x * z.x + z.y * z.y >= 4.0) {
			break;
		}
	}
	float len = pow(iter * 1.0 / k, gamma);
	FragColor = vec4(len * vec3(1), 1.0);
	
}