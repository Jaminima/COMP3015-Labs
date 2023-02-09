#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec4 VertexColor;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec2 VertexTextureCoordinate;

out vec4 Color;
out vec2 vTextureCoordinate;
out vec3 LightIntensity;

uniform mat4 ModelMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

uniform vec4 LightPosition;
uniform vec3 Kd;
uniform vec3 Ld;

void main()
{
    vec3 n = normalize(NormalMatrix * VertexNormal);
    vec4 n_eye = ModelViewMatrix * vec4(n, 1.0f);
    vec3 lightDir = vec3(normalize(LightPosition - n_eye));

    LightIntensity = max(Kd * Ld * dot(lightDir,n), vec3(0.2f));

    Color = VertexColor;
    vTextureCoordinate = VertexTextureCoordinate;

    gl_Position = ModelViewMatrix * vec4(VertexPosition,1.0);
}
