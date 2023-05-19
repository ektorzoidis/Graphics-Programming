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
    return p.x;
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

vec4 waves(vec4 power, float speed, vec2 range)
{
    vec4 wave = vec4(power);
    wave = wave * range.x + wave * range.y * sin(50.0 * time * speed);
    return wave;
}

void main()
{
     //---------------Value noise base---------------
    // Size and default colour
    vec2 noiseUV = vert_In.tC * 5;
    vec3 noiseColour = vec3(1.0);
    vec2 surfaceUV = vert_In.tC + time * 0.025;
    vec3 surfaceColor = texture(diffuse, surfaceUV * 10).rgb;

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
    noiseColour *= vec3(0.81, 0.63, 0.13);
    noiseColour = pow(noiseColour, vec3(3));
    noiseColour += vec3(0.2, 0.0, 0.0);
    if(noiseColour.r > 0.50)
    {
        noiseColour.r = 1.0;
        noiseColour.g = 0.9;
        noiseColour.b = 0.2;
        noiseColour *= 5;
    }
    if(noiseColour.r > 0.45 && noiseColour.r < 0.50)
    {
        noiseColour.r = 1.0;
        noiseColour.g = 0.7;
        noiseColour.b = 0.2;
        noiseColour *= 4;
    }
    if(noiseColour.r > 0.425 && noiseColour.r < 0.45)
    {
        noiseColour.r = 1.0;
        noiseColour.g = 0.4;
        noiseColour.b = 0.2;
        noiseColour *= 3;
    }
    if(noiseColour.r > 0.4 && noiseColour.r < 0.425)
    {
        noiseColour.r = 1.0;
        noiseColour.g = 0.2;
        noiseColour.b = 0.1;
        noiseColour *= 2;
    }
    if (noiseColour.r < 0.4)
    {
        noiseColour = surfaceColor;
    }
    fragcolor = vec4(noiseColour, 1.0);
    vec3 baseNoise = noiseColour;

    //---------------Value noise layer 1---------------
    // Size and default colour
    noiseUV = vert_In.tC * 10;
    noiseColour = vec3(1.0);

    // Add noise layers
    noiseUV -= time * 0.5;
    valueNoise = valueNoiseFn(noiseUV * 4.0) * 1.0;
    valueNoise += valueNoiseFn(noiseUV * 8.0) * 0.5;
    valueNoise += valueNoiseFn(noiseUV * 16.0) * 0.25;
    valueNoise += valueNoiseFn(noiseUV * 32.0) * 0.125;
    valueNoise += valueNoiseFn(noiseUV * 64.0) * 0.0625;
    valueNoise *= 0.6;

    // Set colour
    noiseColour = vec3(valueNoise);
    noiseColour *= vec3(0.81, 0.63, 0.13);
    noiseColour = pow(noiseColour, vec3(2));
    noiseColour += vec3(0.2, 0.0, 0.0);

    fragcolor = mix(fragcolor, vec4(noiseColour, fragcolor.w), 0.7);

    // Wave
    if(noiseColour.r < 0.4)
    {
        float wave = 0.5 + sin(time * 3) / 2;
        wave *= 0.1;
        fragcolor.r -= wave;
        fragcolor.g -= wave;
        fragcolor.b -= wave;
    }

    //---------------Value noise layer 2---------------
    // Size and default colour
    noiseUV = vert_In.tC * 20;
    noiseColour = vec3(1.0);

    // Add noise layers
    noiseUV += time * 1.0;
    valueNoise = valueNoiseFn(noiseUV * 4.0) * 1.0;
    valueNoise += valueNoiseFn(noiseUV * 8.0) * 0.5;
    valueNoise += valueNoiseFn(noiseUV * 16.0) * 0.25;
    valueNoise += valueNoiseFn(noiseUV * 32.0) * 0.125;
    valueNoise += valueNoiseFn(noiseUV * 64.0) * 0.0625;
    valueNoise *= 0.6;

    // Set colour
    noiseColour = vec3(valueNoise);
    noiseColour *= vec3(0.81, 0.63, 0.13);
    noiseColour = pow(noiseColour, vec3(2));
    noiseColour += vec3(0.2, 0.0, 0.0);

    fragcolor = mix(fragcolor, vec4(noiseColour, fragcolor.w), 0.7);

    // Wave
    if(noiseColour.r > 0.5)
    {
        float wave = 0.5 + sin(time * 3) / 2;
        wave *= 0.1;
        fragcolor.r += wave;
        fragcolor.g += wave;
        fragcolor.b += wave;
    }
    if (noiseColour.r < 0.4)
    {
        noiseColour = surfaceColor;
    }

    //---------------Phong lighting---------------
    // Ambient lighting 
    vec3 ambient = vec3(0.5, 0.5, 0.5);

    // Diffuse lighting
    vec3 diffuseNormal = normalize(texture(normal, vert_In.tC + (time * 0.02)).rgb);
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
    lighting = ambient * 0.2 + diffuseColor * 1.0 + specular * 0.7;

    // Set colour
    vec3 phongColour = lighting - 2;
    fragcolor = mix(fragcolor, vec4(phongColour, fragcolor.w), 0.2);

    // Glow
    float intensity = pow(0.5 - dot(vert_In.Normal, vec3(1.0,1.0,1.0)),1.0);
    vec4 glowColour = vec4(1.0, 0.7, 0.2, intensity);
    fragcolor = mix(fragcolor, glowColour, 0.2);
}
