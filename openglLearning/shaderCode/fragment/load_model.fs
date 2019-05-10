#version 330 core

struct Material {
    // 我们也移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色，所以我们不需要将它们分开储存：
    // vec3 ambient;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D emission;
    float shininess;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Position;
in vec3 Normal;

uniform Material material;
uniform samplerCube skybox;
uniform vec3 cameraPos;

void main() {
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    // vec3 R = reflect(I, normalize(Normal));
    vec3 R = refract(I, normalize(Normal), ratio);
    // FragColor = texture(material.texture_diffuse1, TexCoords);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
    // FragColor = vec4(texture(skybox, R).rgb * 0.5 + FragColor.rgb * 0.5, 1.0);
}
