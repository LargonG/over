#version 330 core

in VS_OUT {
	vec4 color;
} fs_in;

void main() {
	gl_FragColor = vec4(1 - fs_in.color.w, 1 - fs_in.color.w, fs_in.color.w, 1.0);

	float gamma = 2.2;
	gl_FragColor.rgb = pow(gl_FragColor.rgb, vec3(1.0 / gamma));
}
