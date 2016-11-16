#version 330 core

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;

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
    vec3 tangent;
    vec3 binormal;
    vec3 worldPos;
    vec3 position;
} IN;

out vec4 gl_FragColor;

vec4 CalcMixedTex ()
{
    const vec3 shadow = vec3 (0, 0, 0);
    const vec3 water = vec3 (0, 0.4, 0.5);
    const vec3 snow = vec3 (1, 1, 1);

    float mix1 = clamp ((IN.position.y - 50.0f) / 100.0f, 0.0f, 1.0f);
    float mix2 = clamp ((IN.position.y - 100.0f) / 150.0f, 0.0f, 1.0f);
    float mix3 = clamp ((IN.position.y) / 100.0f, 0.0f, 1.0f);

    vec4 min = texture (diffuseTex, IN.texCoord);

    vec3 q = mix (shadow, min.rgb, mix1);
    vec3 q1 = mix (q, snow, mix2);
    vec3 q2 = mix (water, q1, mix3);

    return vec4 (q2, min.a);
}

void main ()
{
    vec4 diffuse = CalcMixedTex ();
    mat3 TBN = mat3 (IN.tangent, IN.binormal, IN.normal);
    vec3 normal = normalize (TBN * (texture (bumpTex, IN.texCoord).rgb * 2.0 - 1.0));
    vec3 viewDir = normalize (cameraPos - IN.worldPos);
    int index = 50; // the bigger, the affect of light less

    for (int i = 0; i < lightCount; i++)
    {
        // Calc a direction from this frag to light
        vec3 incident = normalize (lightPos[i] - IN.worldPos);

        // Calc angle between incident and normal
        float lambert = max (0.0, dot (incident, normal));

        // Calc distance from light to this frag
        float dist = length (lightPos[i] - IN.worldPos);

        // Clac if the light can affect this frag
        float atten = 1.0 - clamp (dist / lightRadius[i], 0.0, 1.0);

        // Calc a direction from camera to light
        vec3 halfDir = normalize (incident - viewDir);

        float rFactor = max (0.0, dot (halfDir, normal));

        float sFactor = pow (rFactor, index);
        vec3 colour = (diffuse.rgb * lightColour[i].rgb);
        colour += (lightColour[i].rgb * sFactor) * 0.33;

        gl_FragColor += vec4 (colour * atten * lambert, diffuse.a);
        gl_FragColor.rgb += (diffuse.rgb * lightColour[i].rgb) * 0.1;
    }
}
