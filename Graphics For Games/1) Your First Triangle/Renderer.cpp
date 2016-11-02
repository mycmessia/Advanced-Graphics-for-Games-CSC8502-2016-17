#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent) 
{
	triangle = Mesh::GenerateTriangle ();

	currentShader = new Shader (SHADERDIR"basicVertex.glsl", SHADERDIR"colourFragment.glsl");

	if (!currentShader->LinkProgram()) {
		return;
	}

	init = true;

	glClearColor (0.1f, 0.1f, 0.1f, 1.0f);
}

Renderer::~Renderer(void) 
{
	delete triangle;
}

void Renderer::RenderScene() 
{
	glClear (GL_COLOR_BUFFER_BIT);

	glUseProgram (currentShader->GetProgram());

	//triangle->ChangeColor ();

	triangle->Draw ();

	glUseProgram (0);

	SwapBuffers ();
}

