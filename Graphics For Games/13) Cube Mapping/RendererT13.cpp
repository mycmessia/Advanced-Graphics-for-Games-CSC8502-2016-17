#include "RendererT13.h"

Renderer::Renderer (Window& parent) : OGLRenderer (parent)
{
	camera = new Camera (0.0f, 0.0f, Vector3 (RAW_WIDTH * HEIGHTMAP_X / 2.0f, 500, RAW_HEIGHT * HEIGHTMAP_Z));

	heightMap = new HeightMap (TEXTUREDIR"terrain.raw");
	heightMap->SetTexture (SOIL_load_OGL_texture (TEXTUREDIR"Barren Reds.jpg",
						   SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap (SOIL_load_OGL_texture (TEXTUREDIR"Barren RedsDOT3.jpg",
						   SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!heightMap->GetTexture ())
	{
		return;
	}

	if (!heightMap->GetBumpMap ())
	{
		return;
	}

	quad = Mesh::GenerateQuad (13);
	quad->SetTexture (SOIL_load_OGL_texture (TEXTUREDIR"water.jpg",
					  SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!quad->GetTexture ())
	{
		return;
	}

	reflectShader = new Shader (SHADERDIR"perPixelVertex.glsl", SHADERDIR"reflectFragment.glsl");
	skyboxShader = new Shader (SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");
	lightShader = new Shader (SHADERDIR"perPixelVertex.glsl", SHADERDIR"perPixelFragment.glsl");

	if (!reflectShader->LinkProgram ())
	{
		return;
	}

	if (!skyboxShader->LinkProgram ())
	{
		return;
	}

	if (!lightShader->LinkProgram ())
	{
		return;
	}

	cubeMap = SOIL_load_OGL_cubemap (
		TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
		TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg",
		TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);

	if (!cubeMap)
	{
		return;
{
	delete camera;
	camera = nullptr;

	delete heightMap;
	heightMap = nullptr;

	delete quad;
	quad = nullptr;

	delete reflectShader;
	reflectShader = nullptr;

	delete skyboxShader;
	skyboxShader = nullptr;

	delete lightShader;
	lightShader = nullptr;

	for (unsigned i = 0; i < lightVector.size (); i++)
	{
		delete (lightVector[i]);
		lightVector[i] = nullptr;
	}

	currentShader = 0;
}

void Renderer::UpdateScene (float msec)
{
	camera->UpdateCamera (msec);
	viewMatrix = camera->BuildViewMatrix ();
	waterRotate += msec / 1000.0f;
}

	glDepthMask (GL_TRUE);
}
void Renderer::DrawHeightMap ()
{
	SetMultiLights ();
	glUniform3fv (glGetUniformLocation (currentShader->GetProgram (),
				  "cameraPos"), 1, (float *)& camera->GetPosition ());
	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "diffuseTex"), 0);
	// Set the value to 2 because I use GL_TEXTURE2 to store the bumpTex in Mesh.cpp
	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "bumpTex"), 2);
	modelMatrix.ToIdentity ();
	heightMap->Draw ();
	glUseProgram (0);
}
void Renderer::DrawWater ()
{
	SetMultiLights ();
	glUniform3fv (glGetUniformLocation (currentShader->GetProgram (), "cameraPos"), 1, (float *)& camera->GetPosition ());
	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "diffuseTex"), 0);
	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "cubeTex"), 3);
	glActiveTexture (GL_TEXTURE3);

	float heightY = 256 * HEIGHTMAP_Y / 3.0f;

	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix =
		Matrix4::Translation (Vector3 (heightX, heightY, heightZ)) *
		Matrix4::Scale (Vector3 (heightX, 1, heightZ)) *
		Matrix4::Rotation (90, Vector3 (1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale (Vector3 (10.0f, 10.0f, 10.0f)) * Matrix4::Rotation (waterRotate, Vector3 (0.0f, 0.0f, 1.0f));
	quad->Draw ();
	glUseProgram (0);
}
void Renderer::RenderScene ()
{
	glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox ();
	DrawHeightMap ();
	DrawWater ();

	SwapBuffers ();
}
void Renderer::AddLight (Vector3 position, Vector4 colour, float radius)
{
	if (lightVector.size () < Renderer::MAX_LIGHT_COUNT)
}

{

	{
		ostringstream lcss;
		lcss << "lightColour[" << i << "]";
		string lcstr = lcss.str ();

		glUniform4f (glGetUniformLocation (currentShader->GetProgram (), lcstr.c_str ()),
					 lightColour[i].x, lightColour[i].y, lightColour[i].z, lightColour[i].w);

		ostringstream lpss;
		lpss << "lightPos[" << i << "]";
		string lpstr = lpss.str ();

		glUniform3f (glGetUniformLocation (currentShader->GetProgram (), lpstr.c_str ()),
					 lightPos[i].x, lightPos[i].y, lightPos[i].z);

		ostringstream lrss;
		lrss << "lightRadius[" << i << "]";
		string lrstr = lrss.str ();

		glUniform1f (glGetUniformLocation (currentShader->GetProgram (), lrstr.c_str ()),
					 lightRadius[i]);
	}