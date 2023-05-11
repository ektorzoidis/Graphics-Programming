// Version number
#version 330 core

// Layout qualifers
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

// Uniform variabe
uniform float time;

// Passing in interface block
in VS_OUT
{
    vec2 texCoords;
} vert_In[];

// Passing out
out vec2 TexCoords;

vec4 explode(vec4 position, vec3 normal)
{
    // Amout of explosion
    float magnitude = 1.0;
	// Direction of explosion, going along normal
    vec3 direction = normal * ((sin(time * 1.3) + 1.0) / 2.0) * magnitude; 
	// Returning position
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
    // Getting the normal vector of each vertex
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    // Returns the cross product between the two vectors calculated
    return normalize(cross(a, b));
}

void main()
{
    // Getting normal
    vec3 normal = GetNormal();
    // Setting current vertex position
    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = vert_In[0].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = vert_In[1].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = vert_In[2].texCoords;
    EmitVertex();
    EndPrimitive();
}
