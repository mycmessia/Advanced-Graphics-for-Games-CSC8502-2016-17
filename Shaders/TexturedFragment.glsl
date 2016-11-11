#version 430 core
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
	gl_FragColor = texture (diffuseTex, IN.texCoord) + texture (diffuseTex2, IN.texCoord);
}
