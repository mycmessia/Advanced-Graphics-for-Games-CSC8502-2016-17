#include "Renderer.h"

Renderer::Renderer (Window& parent) : OGLRenderer (parent)
{
	heightMap = new HeightMap (TEXTUREDIR"terrain.raw");
	camera = new Camera ();
	camera->SetPosition (Vector3 (500, 500, 500));
	camera->SetYaw (180.0f);

	currentShader = new Shader (SHADERDIR"TexturedVertex.glsl",
								SHADERDIR"TexturedFragment.glsl");

	if (!currentShader->LinkProgram ())
	{
		return;
	}

	heightMap->SetTexture (SOIL_load_OGL_texture (
		TEXTUREDIR"Barren Reds.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetTexture2 (SOIL_load_OGL_texture (
		TEXTUREDIR"water.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!heightMap->GetTexture ())
	{
		return;
	}

	SetTextureRepeating (heightMap->GetTexture (), true);
	projMatrix = Matrix4::Perspective (1.0f, 10000.0f,
									   (float)width / (float)height, 45.0f);

	glEnable (GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);

	init = true;
}

Renderer::~Renderer (void)
{
	delete heightMap;
	delete camera;
}

void Renderer::UpdateScene (float msec)
{
	camera->UpdateCamera (msec);
	viewMatrix = camera->BuildViewMatrix ();
}

void Renderer::RenderScene ()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram (currentShader->GetProgram ());

	UpdateShaderMatrices ();

	GLint tex1UniformLoc = glGetUniformLocation (currentShader->GetProgram (), "diffuseTex");
	glUniform1i (tex1UniformLoc, 0);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, heightMap->GetTexture ());

	GLint tex2UniformLoc = glGetUniformLocation (currentShader->GetProgram (), "diffuseTex2");
	glUniform1i (tex2UniformLoc, 1);	// this 1 means GL_TEXTURE 1 next line
	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_2D, heightMap->GetTexture2 ());

	heightMap->Draw ();

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, 0);

	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_2D, 0);

	glUseProgram (0);

	SwapBuffers ();
}