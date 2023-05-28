#version 460

in vec2 vTextureCoordinate;

uniform struct textureLayer{
    sampler2D faceTexture;
} textureLayers[10];

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void applyDisintergration(){
    float r = rand(gl_FragCoord.xy);
    if (r < 0.5){
        discard;
    }
}