#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;

out Vertex	{
	vec4 colour;
} OUT;

void main(void)	{
	mat4 mvp 		= projMatrix * viewMatrix * modelMatrix;
	gl_Position		= mvp * vec4(position, 1.0);
	OUT.colour		= colour;
}
