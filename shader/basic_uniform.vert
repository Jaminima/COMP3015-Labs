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
    vec3 n = abs(VertexNormal);
//
//    vec3 lightDir = normalize(vec3(LightPosition - vec4(VertexPosition, 1)));

    LightIntensity = n;

//    vec3 n = normalize(NormalMatrix * VertexNormal);
//
//    vec4 n_eye = ModelViewMatrix * vec4(VertexPosition, 1.0f);
//
//    vec3 lightDir = normalize(vec3(LightPosition - n_eye));
//
//    vec3 lightIntensity = Kd * Ld * max(dot(lightDir,n), 0.1f);

    //LightIntensity = max(lightIntensity, vec3(0.0f));
    //LightIntensity = lightIntensity;

    Color = VertexColor;
    vTextureCoordinate = VertexTextureCoordinate;

    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(VertexPosition,1.0);
}
