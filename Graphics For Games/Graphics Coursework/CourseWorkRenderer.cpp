#include "CourseWorkRenderer.h"

Renderer::Renderer (Window &parent) : OGLRenderer (parent)
{
	camera = new Camera (0.0f, 0.0f, Vector3 (RAW_WIDTH * HEIGHTMAP_X / 2.0f, 500, RAW_HEIGHT * HEIGHTMAP_Z));

	// Generate mesh
	skyboxMesh = Mesh::GenerateQuad (0);
	waterMesh = Mesh::GenerateQuad (0);
	heightMap = new HeightMap (TEXTUREDIR"terrain.raw");

	textShader = new Shader (SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	skyboxShader = new Shader (SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");
	lightShader = new Shader (SHADERDIR"BumpVertex.glsl", SHADERDIR"BumpFragment.glsl");
	particleShader = new Shader ("particleVertex.glsl", "particleFragment.glsl", "particleGeometry.glsl");
	reflectShader = new Shader (SHADERDIR"perPixelVertex.glsl", SHADERDIR"reflectFragment.glsl");

	if (!textShader->LinkProgram ()	|| !skyboxShader->LinkProgram () || 
		!lightShader->LinkProgram () || !particleShader->LinkProgram () ||
		!reflectShader->LinkProgram ())
	{
		return;
	}

	basicFont = new Font (SOIL_load_OGL_texture (TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	
	cubeMap = SOIL_load_OGL_cubemap (
		TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
		TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg",
		TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);

	cubeMap2 = SOIL_load_OGL_cubemap (
		TEXTUREDIR"nightsky_lf.tga", TEXTUREDIR"nightsky_rt.tga",
		TEXTUREDIR"nightsky_up.tga", TEXTUREDIR"nightsky_dn.tga",
		TEXTUREDIR"nightsky_ft.tga", TEXTUREDIR"nightsky_bk.tga",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);

	heightMap->SetTexture (SOIL_load_OGL_texture (TEXTUREDIR"Barren Reds.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap (SOIL_load_OGL_texture (TEXTUREDIR"Barren RedsDOT3.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	waterMesh->SetTexture (SOIL_load_OGL_texture (TEXTUREDIR"water.jpg",
						   SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!cubeMap || !cubeMap2 || !heightMap->GetTexture () || !heightMap->GetBumpMap () ||
		!waterMesh->GetTexture ())
	{
		return;
	}

	SetTextureRepeating (waterMesh->GetTexture (), true);
	SetTextureRepeating (heightMap->GetTexture (), true);
	SetTextureRepeating (heightMap->GetBumpMap (), true);

	AddLight (
		Vector3 ((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0F)),
		Vector4 (1, 1, 1, 1),
		(RAW_WIDTH * HEIGHTMAP_X) / 2.0F
	);

	for (unsigned i = 0; i < lightVector.size (); i++)
	{
		lightColour[i] = lightVector[i]->GetColour ();
		lightPos[i] = lightVector[i]->GetPosition ();
		lightRadius[i] = lightVector[i]->GetRadius ();
	}

	emitter = new ParticleEmitter ();

	isDayTime = true;
	timeCounter = 0.0F;

	waterRotate = 0.0f;

	init = true;
}

Renderer::~Renderer (void)
{
	delete camera;

	delete basicFont;
	delete textShader;

	delete skyboxMesh;
	delete skyboxShader;

	delete lightShader;
	lightShader = nullptr;

	for (unsigned i = 0; i < lightVector.size (); i++)
	{
		delete (lightVector[i]);
		lightVector[i] = nullptr;
	}

	delete particleShader;
	delete emitter;

	delete reflectShader;
	delete waterMesh;

	currentShader = 0;
}

void Renderer::UpdateScene (float msec)
{
	emitter->Update (msec);
	camera->UpdateCamera (msec);
	waterRotate += msec / 1000.0f;
}

void Renderer::RenderScene ()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderSkybox ();

	RenderHeightMap ();

	RenderText ();

	RenderParticle ();

	RenderWater ();

	SwapBuffers ();
}

void Renderer::RenderSkybox ()
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthMask (GL_FALSE);

	SetCurrentShader (skyboxShader);

	glUseProgram (currentShader->GetProgram ());

	modelMatrix.ToIdentity ();
	viewMatrix = camera->BuildViewMatrix ();
	projMatrix = Matrix4::Perspective (1.0f, 15000.0f, (float)width / (float)height, 45.0f);

	UpdateShaderMatrices ();

	GLuint cubeTexLoc = glGetUniformLocation (currentShader->GetProgram (), "cubeTex");
	glUniform1i (cubeTexLoc, 0);

	GLuint cubeTex2Loc = glGetUniformLocation (currentShader->GetProgram (), "cubeTex2");
	glUniform1i (cubeTex2Loc, 1);

	if (isDayTime)
	{
		timeCounter += 0.001f;
		if (timeCounter >= 1.0f)
		{
			isDayTime = false;
		}
	}
	else
	{
		timeCounter -= 0.001f;
		if (timeCounter <= 0.0f)
		{
			isDayTime = true;
		}
	}

	GLuint timeCounterLoc = glGetUniformLocation (currentShader->GetProgram (), "timeCounter");
	glUniform1f (timeCounterLoc, timeCounter);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_CUBE_MAP, cubeMap); // !!! Must use GL_TEXTURE_CUBE_MAP not GL_TEXTURE_2D

	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_CUBE_MAP, cubeMap2);

	skyboxMesh->Draw ();

	modelMatrix.ToIdentity ();
	viewMatrix.ToIdentity ();
	projMatrix.ToIdentity ();

	glUseProgram (0);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);

	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);

	glDepthMask (GL_TRUE);
	glDisable (GL_BLEND);
	glDisable (GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void Renderer::RenderText ()
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);

	SetCurrentShader (textShader);

	glUseProgram (currentShader->GetProgram ());

	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "diffuseTex"), 0);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, basicFont->texture);

	stringstream ss;
	int fps = int (FPS);
	ss << fps;
	string s = ss.str();

	//Render function to encapsulate our font rendering!
	DrawText ("MeiYuchen's Course Work", lightVector[0]->GetPosition (), 64.0f, true);
	DrawText ("FPS:" + s, Vector3 (0, 0, 0), 32.0f);

	glUseProgram (0);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, 0);

	glDisable (GL_BLEND);
}

void Renderer::DrawText (const std::string &text, const Vector3 &position, const float size, const bool perspective)
{
	TextMesh* mesh = new TextMesh (text, *basicFont);

	if (perspective)
	{
		modelMatrix = Matrix4::Translation (position) * Matrix4::Scale (Vector3 (size, size, 1));
		viewMatrix = camera->BuildViewMatrix ();
		projMatrix = Matrix4::Perspective (1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	}
	else
	{
		modelMatrix = Matrix4::Translation (Vector3 (position.x, height - position.y, position.z)) * Matrix4::Scale (Vector3 (size, size, 1));
		viewMatrix.ToIdentity ();
		projMatrix = Matrix4::Orthographic (-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}

	UpdateShaderMatrices ();

	mesh->Draw ();

	modelMatrix.ToIdentity ();
	viewMatrix.ToIdentity ();
	projMatrix.ToIdentity ();

	delete mesh;
}

void Renderer::CalcFPS (float msec)
{
	FPS = 1000.0f / msec;
}

void Renderer::AddLight (Vector3 position, Vector4 colour, float radius)
{
	if (lightVector.size () < Renderer::MAX_LIGHT_COUNT)
	{
		lightVector.push_back (new Light (position, colour, radius));
	}
}

void Renderer::SetMultiLights ()
{
	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "lightCount"), lightVector.size ());

	for (unsigned i = 0; i < lightVector.size (); i++)
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
		glUniform1f (glGetUniformLocation (currentShader->GetProgram (), lrstr.c_str ()), lightRadius[i]);
	}
}

void Renderer::RenderHeightMap ()
{
	glEnable (GL_DEPTH_TEST);

	SetCurrentShader (lightShader);

	glUseProgram (currentShader->GetProgram ());

	SetMultiLights ();

	glUniform3fv (glGetUniformLocation (currentShader->GetProgram (), "cameraPos"), 1, (float *)& camera->GetPosition ());
	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "diffuseTex"), 0);
	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "bumpTex"), 1);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, heightMap->GetTexture ());

	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_2D, heightMap->GetBumpMap ());

	modelMatrix.ToIdentity ();
	viewMatrix = camera->BuildViewMatrix ();
	projMatrix = Matrix4::Perspective (1.0f, 15000.0f, (float)width / (float)height, 45.0f);

	UpdateShaderMatrices ();

	heightMap->Draw ();

	modelMatrix.ToIdentity ();
	viewMatrix.ToIdentity ();
	projMatrix.ToIdentity ();

	glUseProgram (0);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, 0);

	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_2D, 0);

	glDisable (GL_DEPTH_TEST);
}

void Renderer::RenderParticle ()
{
	SetCurrentShader (particleShader);

	glUseProgram (currentShader->GetProgram ());

	modelMatrix.ToIdentity ();
	modelMatrix = modelMatrix.Translation (
		Vector3 ((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 600.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0F))
	);
	viewMatrix = camera->BuildViewMatrix ();
	projMatrix = Matrix4::Perspective (1.0f, 15000.0f, (float)width / (float)height, 45.0f);
		
	UpdateShaderMatrices ();

	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "diffuseTex"), 0);

	SetShaderParticleSize (emitter->GetParticleSize ());
	emitter->SetParticleSize (8.0f);
	emitter->SetParticleVariance (1.0f);
	emitter->SetLaunchParticles (16);
	emitter->SetParticleLifetime (2000.0f);
	emitter->SetParticleSpeed (0.1f);

	// bind texture in it
	emitter->Draw ();

	modelMatrix.ToIdentity ();
	viewMatrix.ToIdentity ();
	projMatrix.ToIdentity ();

	glUseProgram (0);
}

void Renderer::SetShaderParticleSize (float f)
{
	glUniform1f (glGetUniformLocation (currentShader->GetProgram (), "particleSize"), f);
}

void Renderer::RenderWater ()
{
	glEnable (GL_DEPTH_TEST);

	SetCurrentShader (reflectShader);

	glUseProgram (currentShader->GetProgram ());
	
	SetMultiLights ();

	glUniform3fv (glGetUniformLocation (currentShader->GetProgram (), "cameraPos"), 1, (float *)& camera->GetPosition ());
	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "diffuseTex"), 0);
	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "cubeTex"), 1);
	glUniform1i (glGetUniformLocation (currentShader->GetProgram (), "cubeTex2"), 2);
	glUniform1f (glGetUniformLocation (currentShader->GetProgram (), "timeCounter"), timeCounter);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, waterMesh->GetTexture ());

	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_CUBE_MAP, cubeMap);

	glActiveTexture (GL_TEXTURE2);
	glBindTexture (GL_TEXTURE_CUBE_MAP, cubeMap2);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);
	float heightY = 256 * HEIGHTMAP_Y / 3.0f;
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix = Matrix4::Translation (Vector3 (heightX, heightY, heightZ)) *
				  Matrix4::Scale (Vector3 (heightX, 1, heightZ)) * 
				  Matrix4::Rotation (90, Vector3 (1.0f, 0.0f, 1.0f));
	viewMatrix = camera->BuildViewMatrix ();
	projMatrix = Matrix4::Perspective (1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	textureMatrix = Matrix4::Scale (Vector3 (10.0f, 10.0f, 10.0f)) *
					Matrix4::Rotation (waterRotate, Vector3 (0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices ();

	waterMesh->Draw ();

	modelMatrix.ToIdentity ();
	viewMatrix.ToIdentity ();
	projMatrix.ToIdentity ();
	textureMatrix.ToIdentity ();

	glUseProgram (0);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, 0);

	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);

	glActiveTexture (GL_TEXTURE2);
	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
	
	glDisable (GL_DEPTH_TEST);
}