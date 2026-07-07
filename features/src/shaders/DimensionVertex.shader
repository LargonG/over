#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoord;

uniform mat4 model;
uniform mat4 mvp;

void main() {
	fNormal = mat3(transpose(inverse(model))) * vNormal;
	fPosition = vec3(model * vec4(vPosition, 1.0));
	gl_Position = mvp * vec4(vPosition, 1.0);
	fTexCoord = vTexCoord;
}
