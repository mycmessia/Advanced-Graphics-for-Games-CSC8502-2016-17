#include "RendererT12.h"

Renderer::Renderer (Window& parent) : OGLRenderer (parent)
{
	camera = new Camera (0.0f, 0.0f, Vector3 (
		RAW_WIDTH * HEIGHTMAP_X / 2.0f, 500, RAW_HEIGHT * HEIGHTMAP_Z));

	heightMap = new HeightMap (TEXTUREDIR"terrain.raw");
	heightMap->SetTexture (SOIL_load_OGL_texture (TEXTUREDIR"Barren Reds.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap (SOIL_load_OGL_texture (TEXTUREDIR"Barren RedsDOT3.jpg", 
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	currentShader = new Shader (SHADERDIR"BumpVertex.glsl",
								SHADERDIR"BumpFragment.glsl");

	if (!currentShader->LinkProgram ())
	{
		return;
	}

	if (!heightMap->GetTexture ())
	{
		return;
	}

	if (!heightMap->GetBumpMap ())
	{
		return;
	}

	SetTextureRepeating (heightMap->GetTexture (), true);	SetTextureRepeating (heightMap->GetBumpMap (), true);
	AddLight (
		Vector3 ((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
		Vector4 (1, 1, 1, 1), 
		(RAW_WIDTH * HEIGHTMAP_X) / 2.0f
	);

	//AddLight (
	//	Vector3 ((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
	//	Vector4 (1, 0, 1, 1), 
	//	(RAW_WIDTH * HEIGHTMAP_X) / 2.0f
	//);

	for (unsigned i = 0; i < lightVector.size (); i++)
	{
		lightColour[i] = lightVector[i]->GetColour ();
		lightPos[i] = lightVector[i]->GetPosition ();
		lightRadius[i] = lightVector[i]->GetRadius ();
	}

	projMatrix = Matrix4::Perspective (1.0f, 15000.0f,
									   (float)width / (float)height, 45.0f);

	glEnable (GL_DEPTH_TEST);
	init = true;
}Renderer::~Renderer (void)
{
	delete camera;
	camera = nullptr;
	delete heightMap;
	heightMap = nullptr;
	for (unsigned i = 0; i < lightVector.size (); i++)
	{
		delete (lightVector[i]);
		lightVector[i] = nullptr;
	}
}

void Renderer::UpdateScene (float msec)
{
	camera->UpdateCamera (msec);
	viewMatrix = camera->BuildViewMatrix ();
}void Renderer::RenderScene ()
{
	glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram (currentShader->GetProgram ());
	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "diffuseTex"), 0);
	// Set the value to 2 because I use GL_TEXTURE2 to store the bumpTex in Mesh.cpp
	glUniform1i (glGetUniformLocation (currentShader -> GetProgram (),"bumpTex"), 2);

	glUniform3fv (glGetUniformLocation (currentShader->GetProgram (),
				  "cameraPos"), 1, (float *)& camera->GetPosition ());

	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "lightCount"), lightVector.size ());

	UpdateShaderMatrices ();

	for (int i = 0; i < lightVector.size (); i++)
	{
		ostringstream lcss;
		lcss << "lightColour[" << i << "]";
		string lcstr = lcss.str ();

		glUniform4f (glGetUniformLocation (currentShader->GetProgram (), lcstr.c_str ()),
					 lightColour[i].x,  lightColour[i].y,  lightColour[i].z,  lightColour[i].w);

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

	//SetShaderLight (*light);

	heightMap->Draw ();

	glUseProgram (0);

	SwapBuffers ();
}void Renderer::AddLight (Vector3 position, Vector4 colour, float radius){	if (lightVector.size () < Renderer::MAX_LIGHT_COUNT)	{		lightVector.push_back (new Light (position, colour, radius));	}}