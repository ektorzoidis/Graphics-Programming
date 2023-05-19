#version 400

in vec3 vN;
uniform float time;
 
layout (location = 0) out vec4 fragColour;
 
void main() 
{
    float intensity = pow(0.5 - dot(vN, vec3(1.0,1.0,1.0)),1.0);
    float wave = 1.0 + sin(time * 5) / 2;
    fragColour = vec4(1.0, 0.7, 0.2, intensity * 0.07 * wave);
}