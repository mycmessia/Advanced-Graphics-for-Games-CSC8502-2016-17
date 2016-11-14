#version 430 core
// These two will bind to texture unit zero by default
uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex2;

in Vertex {
	vec2 texCoord;
	vec4 colours;
	vec3 position;
} IN;

out vec4 gl_FragColor;

void main (void)
{
	vec4 fragOut1 = texture (diffuseTex, IN.texCoord);
	vec4 fragOut2 = texture (diffuseTex2, IN.texCoord);

	if (fragOut1 != fragOut2 )
	{
		gl_FragColor = fragOut1 + fragOut2;
	}
	else
	{
		gl_FragColor = fragOut1;
	}
}
