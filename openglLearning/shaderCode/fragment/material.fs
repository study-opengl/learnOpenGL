#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec3 LampPos;

out vec4 FragColor;

void main() {
    // 环境关照强度
    float ambientStrength = 0.1f;
    vec3 ambient = light.ambient * material.ambient;
    // 计算漫反射光照
    vec3 lightDir = normalize(LampPos - FragPos);
    vec3 normal = normalize(Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * light.diffuse * material.diffuse;
    // 计算镜面光照
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    // 镜面强度
    float specularStrength = 0.5;
    // 镜面分量计算 32是高光的反光度
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = spec * light.specular * material.specular;
    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0f);
}

