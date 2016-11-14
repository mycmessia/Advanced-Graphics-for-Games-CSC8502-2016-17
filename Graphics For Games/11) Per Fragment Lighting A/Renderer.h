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
	static const int MAX_LIGHT_COUNT = 10;

	Mesh* heightMap;
	Camera* camera;
	vector<Light*> lightVector;
	
	Vector4 lightColour[MAX_LIGHT_COUNT];
	Vector3 lightPos[MAX_LIGHT_COUNT];
	GLfloat lightRadius[MAX_LIGHT_COUNT];
};