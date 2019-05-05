#version 330 core

in vec2 TexCoords;

uniform sampler2D screenTexture;

out vec4 FragColor;

const float offset = 1.0 / 300.0;

void main() {
    vec2 offsets[9] = vec2[](
        vec2(-offset, -offset),
        vec2(0.0, -offset),
        vec2(offset, -offset),
        vec2(-offset, 0.0),
        vec2(0.0, 0.0),
        vec2(offset, 0.0),
        vec2(-offset, offset),
        vec2(0.0, offset),
        vec2(offset, offset)
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1, 9, -1,
        -1, -1, -1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i += 1) {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i += 1) {
        col += sampleTex[i] * kernel[i];
    }

    FragColor = vec4(col, 1.0);
}