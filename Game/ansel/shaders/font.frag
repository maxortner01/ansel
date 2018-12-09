#version 330 core

out vec4 FragColor;

in vec2 tex;

uniform sampler2D texture0;

void main(void) {
    FragColor = vec4(1, 1, 1, texture2D(texture0, tex).r);
}
 