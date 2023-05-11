// Version number
#version 420 core

// Layout qualifer
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TextCoords;
layout (location = 2) in vec3 aNormal;

// Unfrom variable
uniform mat4 model;
uniform mat4 transform;

// Passing out interface block
out VS_OUT
{
    vec2 tC;
    vec3 Normal;
    vec3 Position;
} vs_Out;

void main()
{
    vs_Out.tC = TextCoords;
    vs_Out.Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_Out.Position = vec3(model * vec4(aPos, 1.0));

    gl_Position = transform * vec4(aPos, 1.0);
}
