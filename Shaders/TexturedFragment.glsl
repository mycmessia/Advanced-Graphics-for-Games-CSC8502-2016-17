#version 430 core
uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex2;

in Vertex {
	vec2 texCoord;
	vec4 colours;
	vec3 position;
} IN;

out vec4 gl_FragColor;

const vec3 shadow = vec3 (0, 0, 0);
const vec3 water = vec3 (0, 0.4, 0.5);
const vec3 snow = vec3 (1, 1, 1);

void main (void)
{
	float mix1 = clamp ((IN.position.y - 50.0f) / 100.0f, 0.0f, 1.0f);
	float mix2 = clamp ((IN.position.y - 100.0f) / 150.0f, 0.0f, 1.0f);
	float mix3 = clamp ((IN.position.y) / 100.0f, 0.0f, 1.0f);

	vec4 min = texture (diffuseTex, IN.texCoord);

	vec3 q = mix (shadow, min.rgb, mix1);
	vec3 q1 = mix (q, snow, mix2);
	vec3 q2 = mix (water, q1, mix3);

	gl_FragColor.rgb = q2;


	// gl_FragColor = texture(diffuseTex, IN.texCoord) + texture(diffuseTex2, IN.texCoord);
}
