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

uniform int lights = 1;
uniform bool ignoreTransforms = false;

uniform struct Lighting {
    vec4 ambient;
    vec4 diffuse;
    vec3 Position;
    vec4 ViewPosition;
    vec4 specular;
} Light[10];

uniform struct Material {
    vec4 ambient;
    float shininess;
} Mat;

uniform int toggle_global = 1, toggle_ambient = 1, toggle_diffuse = 1, toggle_specular = 1;

vec4 ProcessLight(int lightID, vec3 N, vec3 V, vec3 view, vec3 normal){
    Lighting _light = Light[lightID];

    vec3 lightPos = normalize(_light.Position - V);

    vec3 h = normalize(view + lightPos);

    vec4 ambient = _light.ambient;
    vec4 global = Mat.ambient * ambient;
    

    vec4 diffuse = _light.diffuse * max (dot(lightPos, N), 0.0);
    diffuse = clamp(diffuse,0,1);

    vec3 reflectD = lightPos + 2*(dot(lightPos,normal)) * normal;
    float dotVR = max(dot(h, normal),0.0);
    float spec = pow(dotVR,Mat.shininess);

    vec4 specular = _light.specular * spec;
    specular = clamp(specular,0.0,1.0);

    return  (toggle_global * global) + (toggle_ambient * ambient) + (toggle_diffuse * diffuse) + (toggle_specular * specular);
}

void main()
{
    if (!ignoreTransforms){
        vec3 N = normalize(NormalMatrix * VertexNormal);
        vec3 V = vec3(ModelViewMatrix * vec4(VertexPosition,1));

        vec3 view = normalize(-V);
        vec3 normal = normalize(N);

        vec4 lightColour = vec4(0);

        for (int i=0;i<lights;i++){
            lightColour+=ProcessLight(i,N,V,view,normal);
        }


        Color = lightColour/lights;
        vTextureCoordinate = VertexTextureCoordinate;

        gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(VertexPosition,1.0);
    }
    else{
        Color = vec4(255);
        vTextureCoordinate = VertexTextureCoordinate;
        gl_Position = mat4(1) * vec4(VertexPosition,1.0);
    }
}
