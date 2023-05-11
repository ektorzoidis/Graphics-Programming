#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 model;
uniform mat4 transform;

out vec3 vN;

void main()
{
	vN = mat3(transpose(inverse(model))) * VertexNormal;
	gl_Position = transform * vec4(VertexPosition, 1.0);
}