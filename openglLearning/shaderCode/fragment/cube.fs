#version 330 core

uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 lampPos;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

void main() {
    // 环境关照强度
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    vec3 lightDir = normalize(lampPos - FragPos);
    vec3 normal = normalize(Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 color = (ambient + diffuse) * cubeColor;
    FragColor = vec4(color, 1.0f);
}
