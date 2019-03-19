#version 330 core

uniform vec3 cubeColor;
uniform vec3 lightColor;

out vec4 FragColor;

void main() {
    // 环境关照强度
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    vec3 color = ambient * cubeColor;
    FragColor = vec4(color, 1.0f);
}
