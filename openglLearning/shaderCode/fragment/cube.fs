#version 330 core

uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec3 LampPos;

out vec4 FragColor;

void main() {
    // 环境关照强度
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    // 计算漫反射光照
    vec3 lightDir = normalize(LampPos - FragPos);
    vec3 normal = normalize(Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // 计算镜面光照
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    // 镜面强度
    float specularStrength = 0.5;
    // 镜面分量计算 32是高光的反光度
    float spec = pow(max(dot(viewDir, reflectDir), 0), 32);
    vec3 specular = specularStrength *  spec * lightColor; 
    vec3 color = (ambient + diffuse + specular) * cubeColor;
    FragColor = vec4(color, 1.0f);
}
