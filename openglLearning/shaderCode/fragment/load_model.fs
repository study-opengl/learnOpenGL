#version 330 core

struct Material {
    // 我们也移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色，所以我们不需要将它们分开储存：
    // vec3 ambient;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D emission;
    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;
uniform vec3 viewPos;

#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];

out vec4 FragColor;

vec3 CalcPointLight(PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.texture_specular1, TexCoords));

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    return (ambient + diffuse + specular) * attenuation;
}

void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], FragPos, normal, viewDir);
    }
    // FragColor = texture(material.texture_diffuse1, TexCoords);
    FragColor = vec4(result, 1.0);
}
