#version 330 core

layout (location = 0) in vec4 position;

out VS_OUT {
  vec4 color;
} vs_out;

layout (std140) uniform Camera {
  mat4 projection;
  mat4 view;
  mat4 model;
};

void main() {
  gl_Position = projection * view * model * vec4(position.xyz, 1.0);
  gl_PointSize = 10;
  vs_out.color = position;
}
