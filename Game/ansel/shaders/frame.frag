#version 330 core

out vec4 FragColor;

in vec2 tex;

uniform sampler2D texture0;

void main(void) {
  //FragColor = vec4(vec3(1.0 - texture(texture0, tex)), 1.0);
  FragColor = texture(texture0, tex);
}
