#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec4 VertexColor;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec2 VertexTextureCoordinate;

out vec4 Color;
out vec2 vTextureCoordinate;
out vec3 LightIntensity;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

uniform struct Lighting {
    vec4 ambient;
    vec4 diffuse;
    vec3 Position;
    vec4 ViewPosition;
    vec4 specular;
} Light;

uniform struct Material {
    vec4 ambient;
    float shininess;
} Mat;

void main()
{
    vec3 N = normalize(NormalMatrix * VertexNormal);
    vec3 V = vec3(ModelViewMatrix * vec4(VertexPosition,1));

    vec3 lightPos = normalize(Light.ViewPosition.xyz - VertexPosition);
    vec3 view = normalize(-V);
    vec3 normal = normalize(N);

    vec4 ambient = Light.ambient;
    vec4 global = Mat.ambient * ambient;

    vec4 diffuse = Light.diffuse * max (dot(lightPos, N), 0.0);
    diffuse = clamp(diffuse,0,1);

    vec3 reflectD = reflect(-lightPos,normal);
    float spec = pow(max(dot(reflectD,normalize(-VertexPosition)),0.0),Mat.shininess);

    vec4 specular = Light.specular * spec;
    specular = clamp(specular,0,1);

    Color = specular; //global + ambient + diffuse + specular;
    vTextureCoordinate = VertexTextureCoordinate;

    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(VertexPosition,1.0);
}
