#version 330 core

in VS_OUT {
	vec3 color;
} fs_in;

void main() {
	gl_FragColor = vec4(fs_in.color, 1.0);

	float gamma = 2.2;
	gl_FragColor.rgb = pow(gl_FragColor.rgb, vec3(1.0 / gamma));
}
