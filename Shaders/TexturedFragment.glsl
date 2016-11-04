#version 430 core
uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex2;

in Vertex {
	vec2 texCoord;
	vec4 colours;
} IN;

out vec4 gl_FragColor;

void main(void){
	gl_FragColor.rgb = IN.colours.rgb * texture(diffuseTex, IN.texCoord).rgb;
}