#version 330 core

layout (location = 0) in vec3 vPosition;

out vec3 fTexCoords;

layout (std140) uniform Camera {
	mat4 projection;
	mat4 view;
};

void main() {
	fTexCoords = vPosition;
	gl_Position = projection * mat4(mat3(view)) * vec4(vPosition, 1.0);
	gl_Position = gl_Position.xyww;
}
