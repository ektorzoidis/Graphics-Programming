// Version number
#version 400

// Layout qualifers
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 TextureCoordinates;
layout (location = 2) in vec3 VertexNormal;

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
    vs_Out.tC = TextureCoordinates;
    vs_Out.Normal = mat3(transpose(inverse(model))) * VertexNormal;
    vs_Out.Position = vec3(model * vec4(VertexPosition, 1.0));

    gl_Position = transform * vec4(VertexPosition, 1.0);
}
