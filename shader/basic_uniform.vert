#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec4 VertexColor;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec2 VertexTextureCoordinate;

out vec4 Color;
out vec2 vTextureCoordinate;

uniform mat4 RotationMatrix;

void main()
{
    Color = VertexColor;
    vTextureCoordinate = VertexTextureCoordinate;
    gl_Position = RotationMatrix * vec4(VertexPosition,1.0);
}
