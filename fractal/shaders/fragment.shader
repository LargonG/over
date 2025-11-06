#version 330 core

in vec3 color;
in vec3 position;

out vec4 FragColor;

uniform vec2 offset;
uniform int k;

void main() {
	vec2 v0 = (position.xy + offset);

	vec2 v = v0;
	for (int i = 0; i < k; i++) {
		vec2 nv = vec2(
			v.x * v.x - v.y * v.y + v0.x,
			2 * v.x * v.y + v0.y
			);
		v = nv;
	}
	float len = length(v);
	if (len > 0) {
		len = 1;
	}
	FragColor = vec4(len, len, len, 1.0);
}