// Version number
#version 400

// Unfrom variable
uniform vec3 cameraPos;
uniform sampler2D diffuse;
uniform sampler2D normal;
uniform float time;

// Passing out
out vec4 fragcolor;

// Passing in interface block
in VS_OUT 
{
    vec2 tC;
    vec3 Normal;
    vec3 Position;
} vert_In;

vec2 quintic(vec2 p)
{
    return p * p * p * (10.0 + p * (-15.0 + p * 6.0));
}

float whiteNoise2x1(vec2 p) 
{
    // Generic noise function
    float random = dot(p, vec2(12.0, 78.0));
    random = sin(random);
    random = random * 43758.5453;
    random = fract(random);
    return random;
}

float valueNoiseFn(vec2 uv) {
  vec2 gridUv = fract(uv);
  vec2 gridId = floor(uv);

  gridUv = quintic(gridUv);

  float botLeft = whiteNoise2x1(gridId);
  float botRight = whiteNoise2x1(gridId + vec2(1.0, 0.0));
  float b = mix(botLeft, botRight, gridUv.x);

  float topLeft = whiteNoise2x1(gridId + vec2(0.0, 1.0));
  float topRight = whiteNoise2x1(gridId + vec2(1.0, 1.0));
  float t = mix(topLeft, topRight, gridUv.x);

  float noise = mix(b, t, gridUv.y);

  return noise;
}

void main()
{
     //---------------Value noise base---------------
    // Size and default colour
    vec2 noiseUV = vert_In.tC * 10;
    vec3 noiseColour = vec3(1.0);

    // Add noise layers
    noiseUV += time * 0.5;
    float valueNoise = valueNoiseFn(noiseUV * 4.0) * 1.0;
    valueNoise += valueNoiseFn(noiseUV * 8.0) * 0.5;
    valueNoise += valueNoiseFn(noiseUV * 16.0) * 0.25;
    valueNoise += valueNoiseFn(noiseUV * 32.0) * 0.125;
    valueNoise += valueNoiseFn(noiseUV * 64.0) * 0.0625;
    valueNoise *= 0.6;

    // Set colour
    noiseColour = vec3(valueNoise);
    noiseColour *= vec3(0.2, 0.7, 0.7);
    noiseColour = pow(noiseColour, vec3(3));
    fragcolor = vec4(noiseColour, 1.0);

    //---------------Phong lighting---------------
    // Ambient lighting 
    vec3 ambient = vec3(0.5, 0.5, 0.5);

    // Diffuse lighting
    vec3 diffuseNormal = texture(normal, vert_In.tC + (time * 0.03)).rgb;
	diffuseNormal = normalize(diffuseNormal * 2.0 - 1.0); 
    vec3 lightColour = vec3(1.0, 0.7, 0.2);
    vec3 lightPos = vec3(5.0, 5.0, -5.0);
    float diffuseStrength = max(dot(diffuseNormal, lightPos), 0.0);
    vec3 diffuseColor = diffuseStrength * lightColour;

    // Specular light
    vec3 camera = normalize(cameraPos);
    vec3 reflection = normalize(reflect(-lightPos, diffuseNormal));
    float specularStrength = max(0.0, dot(camera, reflection));
    specularStrength = pow(specularStrength, 256.0);
    vec3 specular = specularStrength * lightColour;

    // Final lighting
    vec3 lighting = vec3(0.0, 0.0, 0.0);
    lighting = ambient * 0.2 + diffuseColor * 0.8 + specular * 1.0;

    // Set colour
    vec3 modelColour = texture(diffuse, vert_In.tC + (time * 0.03)).rgb;
    vec3 phongColour = modelColour * (lighting + 1);
    fragcolor = mix(fragcolor, vec4(phongColour, fragcolor.w), 0.8);

    // Glow
    float intensity = pow(0.5 - dot(vert_In.Normal, vec3(1.0,1.0,1.0)),1.0);
    vec4 glowColour = vec4(1.0, 0.7, 0.2, intensity);
    fragcolor = mix(fragcolor, glowColour, 0.001);
}
