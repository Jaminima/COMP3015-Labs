#version 460

in vec4 Color;
in vec3 LightIntensity;
in vec2 vTextureCoordinate;
layout (location = 0) out vec4 FragColor;

uniform int activeTextureLayers = 0;

uniform struct textureLayer{
    sampler2D faceTexture;
} textureLayers[10];

uniform int toonBands = 10;
uniform bool enableToon = false;
uniform bool enableEdge = false;

vec4 toon(vec4 c){
    c *= toonBands;

    c = round(c);

    c /= toonBands;

    return c;
}

vec4 mergeTextures(){
    if (activeTextureLayers==0) return vec4(1);

    vec3 texCum = vec3(0);
    vec4 tex;
    for (int i=0;i<activeTextureLayers;i++){
        tex = texture(textureLayers[i].faceTexture, vTextureCoordinate);

        texCum = mix(texCum.rgb,tex.rgb,tex.a);
    }
    return vec4(texCum/activeTextureLayers,1);
}

vec4 calculateEdge();
vec4 calculateGausian();

void main() {
    FragColor = mergeTextures() * Color;

    if (enableEdge){
        //FragColor = calculateEdge();
        FragColor = calculateGausian();
    }

    else if (enableToon){
        FragColor = toon(FragColor);
    }
}
