#version 460

in vec2 vTextureCoordinate;

uniform float time=0;
uniform bool disintergrate=false;

uniform struct textureLayer{
    sampler2D faceTexture;
} textureLayers[10];

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void applyDisintergration(){
    float r = rand(gl_FragCoord.xy);
    float t = 10/(time*time);
    if (disintergrate && (r > t || t < 0.01)){
        discard;
    }
}