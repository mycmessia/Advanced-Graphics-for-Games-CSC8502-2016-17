#version 330 core

uniform samplerCube cubeTex;
uniform samplerCube cubeTex2;	// Must be used in main func or it will be deleted by compiler

uniform float timeCounter;		// from 0.0f to 1.0f

in Vertex {
	vec3 normal;
} IN;

out vec4 gl_FragColor;

void main (void)
{
	gl_FragColor = mix (
        texture (cubeTex, normalize (IN.normal)),
        texture (cubeTex2, normalize (IN.normal)),
        timeCounter
    );
}
