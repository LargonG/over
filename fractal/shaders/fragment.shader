#version 330 core

in vec3 color;
in vec3 position;

out vec4 FragColor;

uniform vec2 offset;
uniform int k;
uniform float gamma;

void main() {
	vec2 v0 = (position.xy + offset);
	
	float rho = length(v0 - vec2(0.25, 0));
	float theta = atan(v0.y, v0.x - 0.25);
	float rho_c = 0.5 - 0.5 * cos(theta);

	int iter = k;
	if (rho > rho_c) {
		vec2 v = vec2(0, 0);
		for (iter = 0; iter < k; iter++) {
			v = vec2(
				v.x * v.x - v.y * v.y + v0.x,
				2 * v.x * v.y + v0.y
				);
			if (v.x * v.x + v.y * v.y >= 4.0) {
				break;
			}
		}
	}
	float len = pow(iter * 1.0 / k, gamma);
	FragColor = vec4(len * vec3(1), 1.0);
	
}