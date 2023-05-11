// Version Number
#version 400

// Layout qualifers
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 TextCoords;
layout (location = 2) in vec3 VertexNormal;

// Uniform variable
uniform mat4 transform;

// Passing out interface block
out VS_OUT
{
    vec2 texCoords;
} vs_Out;

// Passing out
out vec3 v_norm;
out vec2 v_textCoords;
out vec4 v_pos; 

void main()
{
	// Assigning the normal and position data
	v_norm = VertexNormal;
	v_pos = vec4(VertexPosition, 1.0);
	vs_Out.texCoords = TextCoords;

	// Sets the position of the current vertex
	gl_Position = transform * vec4(VertexPosition, 1.0);
}
