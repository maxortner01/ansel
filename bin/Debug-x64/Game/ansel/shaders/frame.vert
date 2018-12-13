#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 6) in vec2 texCoords;

out vec2 tex;

void main(void) {
  gl_Position = vec4(vertex.x, vertex.y, 0.0, 1.0);
  tex = texCoords;
}
