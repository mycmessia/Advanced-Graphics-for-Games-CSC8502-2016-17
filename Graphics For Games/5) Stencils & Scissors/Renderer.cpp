#include "Renderer.h"

Renderer::Renderer (Window & parent) : OGLRenderer (parent)
{
	triangle = Mesh::GenerateTriangle (5);
	quad = Mesh::GenerateQuad (5);

	currentShader = new Shader (SHADERDIR"texturedVertex.glsl",
								SHADERDIR"stencilFragment.glsl");

	if (!currentShader -> LinkProgram ())
	{
		return;
	}

	triangle -> SetTexture (SOIL_load_OGL_texture (TEXTUREDIR"brick.tga",
							 SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
	quad -> SetTexture (SOIL_load_OGL_texture (TEXTUREDIR"chessboard.tga",
						 SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

	if (!triangle -> GetTexture () || !quad -> GetTexture ())
	{
		return;
	}

	usingScissor = false;
	usingStencil = false;
	init = true;
}

Renderer::~Renderer (void)
{
	delete triangle;
	delete quad;
}void Renderer::ToggleScissor ()
{
	usingScissor = !usingScissor;
}

void Renderer::ToggleStencil ()
{
	usingStencil = !usingStencil;
}void Renderer::RenderScene ()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (usingScissor)
	{
		glEnable (GL_SCISSOR_TEST);
		glScissor ((float)width / 2.5f, (float)height / 2.5f,
				   (float)width / 5.0f, (float)height / 5.0f);
	}	glUseProgram (currentShader -> GetProgram ());
	UpdateShaderMatrices ();

	glUniform1i (glGetUniformLocation (currentShader -> GetProgram (), "diffuseTex"), 0);

	if (usingStencil)
	{
		glEnable (GL_STENCIL_TEST);

		glColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc (GL_ALWAYS, 1, 0xFF);
		glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);

		glBindTexture (GL_TEXTURE_2D, quad->GetTexture ());
		quad -> Draw ();
		glBindTexture (GL_TEXTURE_2D, 0);

		glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		// stencil the black chessboard pieces GL_EQUAL
		// stencil the white chessboard pieces GL_NOTEQUAL
		glStencilFunc (GL_EQUAL, 1, 0xFF);
		glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
	}	glBindTexture (GL_TEXTURE_2D, triangle->GetTexture ());	triangle -> Draw ();
	glBindTexture (GL_TEXTURE_2D, 0);

	glUseProgram (0);
	glDisable (GL_SCISSOR_TEST);
	glDisable (GL_STENCIL_TEST);

	SwapBuffers ();
}