#version 330 core

uniform vec3 cameraPos;
uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;
uniform samplerCube cubeTex2;

uniform float timeCounter;

const int MAX_LIGHT_COUNT = 10;
uniform int lightCount;

uniform vec4 lightColour[MAX_LIGHT_COUNT];
uniform vec3 lightPos[MAX_LIGHT_COUNT];
uniform float lightRadius[MAX_LIGHT_COUNT];

in Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
} IN;

out vec4 gl_FragColor;

void main (void)
{
    vec4 diffuse = texture (diffuseTex, IN.texCoord) * IN.colour;
    vec3 incident = normalize (IN.worldPos - cameraPos);

    for (int i = 0; i < lightCount; i++)
    {
        float dist = length (lightPos[i] - IN.worldPos);
        float atten = 1.0 - clamp (dist / lightRadius[i], 0.2, 1.0);
        vec4 reflection = mix (
            texture (cubeTex, reflect (incident, normalize (IN.normal))),
            texture (cubeTex2, reflect (incident, normalize (IN.normal))),
            timeCounter
        );

        gl_FragColor += (lightColour[i] * diffuse * atten) * (diffuse + reflection);
    }
}
