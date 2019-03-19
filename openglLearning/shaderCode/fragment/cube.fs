#version 330 core

uniform vec3 cubeColor;
uniform vec3 lightColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(lightColor * cubeColor, 1.0f);
}