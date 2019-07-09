#version 330 core

struct Material {
    // 我们也移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色，所以我们不需要将它们分开储存：
    // vec3 ambient;
    sampler2D texture_diffuse1;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

out vec4 FragColor;

in vec2 TexCoords;

uniform Material material;

void main() {
    FragColor = texture(material.texture_diffuse1, TexCoords);
}
