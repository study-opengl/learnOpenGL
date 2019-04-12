
#version 330 core

struct Material {
    // 我们也移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色，所以我们不需要将它们分开储存：
    // vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction; // 聚光所指向的方向
    float cutOff; // 内切光角余弦值
    float outterCutOff; // 外切光角余弦值

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec3 LampPos;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outterCutOff;
    float intensity = clamp((theta - light.outterCutOff) / epsilon, 0, 1.0);

    // 环境关照强度
    float ambientStrength = 0.1f;
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    // 计算漫反射光照
    vec3 normal = normalize(Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * light.diffuse * texture(material.diffuse, TexCoords).rgb;
    // 计算镜面光照
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    // 镜面强度
    float specularStrength = 0.5;
    // 镜面分量计算 32是高光的反光度
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = spec * light.specular * texture(material.specular, TexCoords).rgb;

    // 放射光贴图(Emission Map)
    vec3 emission = texture(material.emission, TexCoords).rgb;

    // 实现衰减
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    vec3 color = ambient + (diffuse + specular) * attenuation * intensity + emission * intensity;
    FragColor = vec4(color, 1.0f);
}
