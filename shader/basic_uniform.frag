#version 460

in vec4 Color;
in vec3 LightIntensity;
in vec2 vTextureCoordinate;
layout (location = 0) out vec4 FragColor;

uniform sampler2D faceTexture;

void main() {
    //FragColor = texture(faceTexture, vTextureCoordinate) * Color;
    FragColor = Color * vec4(LightIntensity, 1.0f);
}
