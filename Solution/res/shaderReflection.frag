// Version number
#version 420 core

// Layout qualifer
layout (binding = 1) uniform sampler2D diffuse;

// Unfrom variable
uniform vec3 cameraPos;
uniform samplerCube skybox;

// Passing out
out vec4 fragcolor;

// Passing in interface block
in VS_OUT
{
    vec2 tC;
    vec3 Normal;
    vec3 Position;
} vert_In;

void main()
{
    // Vector to camera
    vec3 I = normalize(vert_In.Position - cameraPos);
    // Normalise normals
    vec3 N = normalize(vert_In.Normal);
    // Calculate the reflection direction for the incident vector
    vec3 R = reflect(I, N);
    vec4 Colour = vec4(texture(skybox, R).rgb, 1.0);
    fragcolor = Colour;
}
