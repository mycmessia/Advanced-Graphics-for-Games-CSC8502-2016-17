#include "Renderer.h"

Renderer::Renderer (Window & parent) : OGLRenderer (parent)
{
	triangle = Mesh::GenerateTriangle (3);

	triangle->SetTexture (SOIL_load_OGL_texture (TEXTUREDIR"brick.tga",
						  SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

	triangle->SetTexture2 (SOIL_load_OGL_texture (TEXTUREDIR"Barren RedsDOT3.jpg",
						   SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

	if (!triangle->GetTexture ())
	{
		return;
	}

	currentShader = new Shader (SHADERDIR"texturedVertex.glsl", SHADERDIR"texturedfragment.glsl");

	if (!currentShader->LinkProgram ())
	{
		return;
	}

	init = true;

	projMatrix = Matrix4::Orthographic (-1, 1, 1, -1, 1, -1);

	filtering = true;
	repeating = false;
}

Renderer::~Renderer (void)
{
	delete triangle;
}

void Renderer::RenderScene ()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram (currentShader->GetProgram ());

	UpdateShaderMatrices ();

	GLint tex1UniformLoc = glGetUniformLocation (currentShader->GetProgram (), "diffuseTex");
	glUniform1i (tex1UniformLoc, 0);

	GLint tex2UniformLoc = glGetUniformLocation (currentShader->GetProgram (), "diffuseTex2");
	glUniform1i (tex2UniformLoc, 1);	// this 1 means GL_TEXTURE 1 next line

	triangle->Draw ();

	glUseProgram (0);
	SwapBuffers ();
}

void Renderer::UpdateTextureMatrix (float value)
{
	Matrix4 pushPos = Matrix4::Translation (Vector3 (0.5f, 0.5f, 0));
	Matrix4 popPos = Matrix4::Translation (Vector3 (-0.5f, -0.5f, 0));
	Matrix4 rotation = Matrix4::Rotation (value, Vector3 (0, 0, 1));
	textureMatrix = pushPos * rotation * popPos;
}

void Renderer::ToggleRepeating ()
{
	repeating = !repeating;

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, triangle->GetTexture ());

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,		//x axis
					 repeating ? GL_REPEAT : GL_CLAMP);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,		//y axis
					 repeating ? GL_REPEAT : GL_CLAMP);

	glBindTexture (GL_TEXTURE_2D, 0);

	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_2D, triangle->GetTexture2 ());

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,		//x axis
					 repeating ? GL_REPEAT : GL_CLAMP);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,		//y axis
					 repeating ? GL_REPEAT : GL_CLAMP);

	glBindTexture (GL_TEXTURE_2D, 0);
}

void Renderer::ToggleFiltering ()
{
	filtering = !filtering;

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, triangle->GetTexture ());

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					 filtering ? GL_LINEAR : GL_NEAREST);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
					 filtering ? GL_LINEAR : GL_NEAREST);

	glBindTexture (GL_TEXTURE_2D, 0);

	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_2D, triangle->GetTexture2 ());

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					 filtering ? GL_LINEAR : GL_NEAREST);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
					 filtering ? GL_LINEAR : GL_NEAREST);

	glBindTexture (GL_TEXTURE_2D, 0);
}
