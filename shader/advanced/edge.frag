#version 460

in vec2 vTextureCoordinate;

uniform struct textureLayer{
    sampler2D faceTexture;
} textureLayers[10];

vec4 calculateEdge() {
    ivec2 texSize = textureSize(textureLayers[0].faceTexture,0);

    float w = 1.0 / texSize.x;
    float h = 1.0 / texSize.y;

    vec4[9] pixels;

    pixels[0] = texture(textureLayers[0].faceTexture, vTextureCoordinate + vec2(-w,-h));
    pixels[1] = texture(textureLayers[0].faceTexture, vTextureCoordinate + vec2(-w,0));
    pixels[2] = texture(textureLayers[0].faceTexture, vTextureCoordinate + vec2(-w,h));

    pixels[3] = texture(textureLayers[0].faceTexture, vTextureCoordinate + vec2(-w,0));
    pixels[4] = texture(textureLayers[0].faceTexture, vTextureCoordinate);
    pixels[5] = texture(textureLayers[0].faceTexture, vTextureCoordinate + vec2(w,0));
    
    pixels[6] = texture(textureLayers[0].faceTexture, vTextureCoordinate + vec2(-w,h));
    pixels[7] = texture(textureLayers[0].faceTexture, vTextureCoordinate + vec2(0,h));
    pixels[8] = texture(textureLayers[0].faceTexture, vTextureCoordinate + vec2(w,h));

    
	vec4 sobel_edge_h = pixels[2] + (2.0*pixels[5]) + pixels[8] - (pixels[0] + (2.0*pixels[3]) + pixels[6]);
  	vec4 sobel_edge_v = pixels[0] + (2.0*pixels[1]) + pixels[2] - (pixels[6] + (2.0*pixels[7]) + pixels[8]);
	vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

    return vec4( 1.0 - sobel.rgb, 1.0);
}
