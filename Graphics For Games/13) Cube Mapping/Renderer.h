#pragma once

#include <sstream>

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/HeightMap.h"

class Renderer : public OGLRenderer
{
public:
	Renderer (Window& parent);
	virtual ~Renderer (void);

	virtual void RenderScene ();
	virtual void UpdateScene (float msec);

protected:
	void DrawHeightMap ();
	void DrawWater ();
	void DrawSkybox ();

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;

	Mesh* heightMap;
	Mesh* quad;

	Camera* camera;

	GLuint cubeMap;

	float waterRotate;

	// multi lights
	static const int MAX_LIGHT_COUNT = 10;

	vector<Light*> lightVector;
	
	Vector4 lightColour[MAX_LIGHT_COUNT];
	Vector3 lightPos[MAX_LIGHT_COUNT];
	GLfloat lightRadius[MAX_LIGHT_COUNT];

	void AddLight (Vector3 position, Vector4 colour, float radius);

	void SetMultiLights ();
};