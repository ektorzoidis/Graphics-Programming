// Version number
#version 400

// Layout qualifers
layout (location = 0) out vec4 fragcolor;

// Unfrom variable
uniform sampler2D diffuse;

// Passing in
in vec2 TexCoords;
 
void main()
{
    fragcolor = texture2D(diffuse, TexCoords);
}
