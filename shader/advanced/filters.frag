#version 460

in vec2 vTextureCoordinate;

uniform struct textureLayer{
    sampler2D faceTexture;
} textureLayers[10];

void collectPixels(inout vec4 pixels[9], vec2 idx){
    ivec2 texSize = textureSize(textureLayers[0].faceTexture,0);

    float w = 1.0 / texSize.x;
    float h = 1.0 / texSize.y;

    pixels[0] = texture(textureLayers[0].faceTexture, idx + vec2(-w,-h));
    pixels[1] = texture(textureLayers[0].faceTexture, idx + vec2(-w,0));
    pixels[2] = texture(textureLayers[0].faceTexture, idx + vec2(-w,h));

    pixels[3] = texture(textureLayers[0].faceTexture, idx + vec2(-w,0));
    pixels[4] = texture(textureLayers[0].faceTexture, idx);
    pixels[5] = texture(textureLayers[0].faceTexture, idx + vec2(w,0));
    
    pixels[6] = texture(textureLayers[0].faceTexture, idx + vec2(-w,h));
    pixels[7] = texture(textureLayers[0].faceTexture, idx + vec2(0,h));
    pixels[8] = texture(textureLayers[0].faceTexture, idx + vec2(w,h));
}

vec4 calculateGausian(vec2 idx){
    vec4[9] pixels;
    collectPixels(pixels, idx);

    return (pixels[4] * 4 +
        (pixels[3] + pixels[5] + pixels[1] + pixels[7]) * 2 +
        (pixels[0] + pixels[2] + pixels[6] + pixels[8])
        ) / 16;
}

vec4 calculateEdge(vec4 pixels[9]) {
	vec4 sobel_edge_h = pixels[2] + (2.0*pixels[5]) + pixels[8] - (pixels[0] + (2.0*pixels[3]) + pixels[6]);
  	vec4 sobel_edge_v = pixels[0] + (2.0*pixels[1]) + pixels[2] - (pixels[6] + (2.0*pixels[7]) + pixels[8]);
	vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

    return vec4( 1.0 - sobel.rgb, 1.0);
}

vec4 calculateEdge(vec2 idx){
    vec4[9] pixels;
    collectPixels(pixels, idx);

    return calculateEdge(pixels);
}

vec4 calculateEdgeWithGausian(vec2 idx){
    ivec2 texSize = textureSize(textureLayers[0].faceTexture,0);

    float w = 1.0 / texSize.x;
    float h = 1.0 / texSize.y;

    vec4[9] pixels;

    pixels[0] = calculateGausian(idx + vec2(-w,-h));
    pixels[1] = calculateGausian(idx + vec2(0,-h));
    pixels[2] = calculateGausian(idx + vec2(w,-h));
    
    pixels[3] = calculateGausian(idx + vec2(-w,0));
    pixels[4] = calculateGausian(idx);
    pixels[5] = calculateGausian(idx + vec2(w,0));
    
    pixels[6] = calculateGausian(idx + vec2(-w,h));
    pixels[7] = calculateGausian(idx + vec2(0,h));
    pixels[8] = calculateGausian(idx + vec2(w,h));

    return calculateEdge(pixels);
}
