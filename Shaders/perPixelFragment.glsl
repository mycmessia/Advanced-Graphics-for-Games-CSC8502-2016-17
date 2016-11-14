#version 330 core

uniform sampler2D diffuseTex;
uniform vec3 cameraPos;

const int MAX_LIGHT_COUNT = 10;

uniform int lightCount;

uniform vec4 lightColour[MAX_LIGHT_COUNT];
uniform vec3 lightPos[MAX_LIGHT_COUNT];
uniform float lightRadius[MAX_LIGHT_COUNT];

in Vertex {
    vec3 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
} IN;

out vec4 gl_FragColor;

void main (void)
{
    vec4 diffuse = texture (diffuseTex, IN.texCoord);
    vec3 viewDir = normalize (cameraPos - IN.worldPos);

    for (int i = 0; i < lightCount; i++)
    {
        vec3 incident = normalize (lightPos[i] - IN.worldPos);
        float lambert = max (0.0, dot (incident, IN.normal));
        float dist = length (lightPos[i] - IN.worldPos);
        float atten = 1.0 - clamp (dist / lightRadius[i], 0.0, 1.0);

        vec3 halfDir = normalize (incident - viewDir);
        float rFactor = max (0.0, dot (halfDir, IN.normal));
        float sFactor = pow (rFactor, 50.0);
        vec3 colour = (diffuse.rgb * lightColour[i].rgb);
        colour += (lightColour[i].rgb * sFactor) * 0.33;

        gl_FragColor += vec4 (colour * atten * lambert, diffuse.a);
        gl_FragColor.rgb += (diffuse.rgb * lightColour[i].rgb) * 0.1;
    }
}
