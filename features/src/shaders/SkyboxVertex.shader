#version 330 core

layout (location = 0) in vec3 vPosition;

out vec3 fTexCoords;

uniform mat4 mvp;

void main() {
	fTexCoords = vPosition;
	gl_Position = mvp * vec4(vPosition, 1.0);
	gl_Position = gl_Position.xyww;
}
