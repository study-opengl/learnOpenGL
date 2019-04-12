
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

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outterCutOff;

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
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

in vec3 Normal;
in vec3 FragPos;
in vec3 LampPos;
in vec2 TexCoords;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 fragPos, vec3 normal, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

    // 放射光贴图(Emission Map)
    vec3 emission = texture(material.emission, TexCoords).rgb;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outterCutOff;
    float intensity = clamp((theta - light.outterCutOff) / epsilon, 0.0, 1.0);

    return ambient + (diffuse + specular) * attenuation * intensity + emission * intensity;
}

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(dirLight, normal, viewDir);
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], FragPos, normal, viewDir);
    }
    result += CalcSpotLight(spotLight, FragPos, normal, viewDir);
    FragColor = vec4(result, 1.0);
}
