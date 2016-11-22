#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/HeightMap.h"
#include "../../nclgl/Light.h"
#include "TextMesh.h"

#include <sstream>

class Renderer : public OGLRenderer
{
public:
	Renderer (Window &parent);
	virtual ~Renderer (void);

	virtual void RenderScene (float msec);
	virtual void UpdateScene (float msec);

protected:
	void DrawText (const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);
	void RenderText (float msec);
	void RenderSkybox ();
	void RenderHeightMap ();

	Camera* camera;

	// Render text
	Font* basicFont;
	Shader* textShader;

	// Render skybox (cubeMap)
	GLuint cubeMap;
	Mesh* skyboxMesh;
	Shader* skyboxShader;

	// Render HeightMap
	Mesh* heightMap;
	Shader* heightMapShader;

	// multi lights
	static const int MAX_LIGHT_COUNT = 10;
	Shader* lightShader;
	vector<Light*> lightVector;

	Vector4 lightColour[MAX_LIGHT_COUNT];
	Vector3 lightPos[MAX_LIGHT_COUNT];
	GLfloat lightRadius[MAX_LIGHT_COUNT];

	void AddLight (Vector3 pos, Vector4 colour, float radius);
	void SetMultiLights ();
};