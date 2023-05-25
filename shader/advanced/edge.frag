#version 460

in vec2 vTextureCoordinate;

uniform struct textureLayer{
    sampler2D faceTexture;
} textureLayers[10];

vec4 calculateEdge() {
    return texture(textureLayers[0].faceTexture, vTextureCoordinate);
}
