#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTextureCoordinate;

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

    vec3 lightPos = normalize(Light.Position - V);
    vec3 view = normalize(-V);
    vec3 normal = normalize(N);

    vec3 h = normalize(view + lightPos);

    vec4 ambient = Light.ambient;
    vec4 global = Mat.ambient * ambient;

    vec4 diffuse = Light.diffuse * max (dot(lightPos, N), 0.0);
    diffuse = clamp(diffuse,0,1);

    vec3 reflectD = lightPos + 2*(dot(lightPos,normal)) * normal;
    float dotVR = max(dot(h, normal),0.0);
    float spec = pow(dotVR,Mat.shininess);

    vec4 specular = Light.specular * spec;
    specular = clamp(specular,0.0,1.0);

    Color = global + ambient + diffuse + specular;
    vTextureCoordinate = VertexTextureCoordinate;

    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(VertexPosition,1.0);
}
