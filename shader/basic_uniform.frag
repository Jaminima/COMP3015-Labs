#version 460

in vec4 Color;
in vec2 vTextureCoordinate;
layout (location = 0) out vec4 FragColor;

uniform sampler2D faceTexture;

void main() {
    FragColor = texture(faceTexture, vTextureCoordinate)  * Color;
}
