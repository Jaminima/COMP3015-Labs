#version 460

in vec4 Color;
in vec3 LightIntensity;
in vec2 vTextureCoordinate;
layout (location = 0) out vec4 FragColor;

uniform sampler2D faceTexture;
uniform int toonBands = 10;
uniform bool enableToon = false;

vec4 toon(vec4 c){
    c *= toonBands;

    c = round(c);

    c /= toonBands;

    return c;
}

void main() {
    vec4 tex = texture(faceTexture, vTextureCoordinate);
    
    if (all(equal(tex.rgb,vec3(0)))) tex = vec4(1);

    FragColor = tex * Color;

    if (enableToon){
        FragColor = toon(FragColor);
    }
}
