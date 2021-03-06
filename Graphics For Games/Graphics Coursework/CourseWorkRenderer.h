#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/HeightMap.h"
#include "../../nclgl/Light.h"
#include "../../nclgl/MD5Mesh.h"
#include "../../nclgl/MD5Node.h"
#include "TextMesh.h"
#include "ParticleEmitter.h"
#include "VolcanoParticleEmitter.h"
#include "SnowParticleEmitter.h"

#include <sstream>

#define SHADOWSIZE 2048

class Renderer : public OGLRenderer
{
public:
	Renderer (Window &parent);
	virtual ~Renderer (void);

	virtual void RenderScene ();
	virtual void UpdateScene (float msec);

	void CalcFPS (float msec);

protected:
	Camera* camera;

	// Render text
	float FPS;
	Font* basicFont;
	Shader* textShader;
	
	void DrawText (const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);
	void RenderText ();

	// Render skybox (cubeMap)
	bool isDayTime;
	GLfloat timeCounter;	// User to simulate time day2night night2day
	GLuint cubeMap;			// day
	GLuint cubeMap2;		// night
	Mesh* skyboxMesh;
	Shader* skyboxShader;

	void RenderSkybox ();

	// Render HeightMap Multi lights
	Mesh* heightMap;
	Shader* heightMapShader;

	static const int MAX_LIGHT_COUNT = 10;
	Shader* lightShader;
	vector<Light*> lightVector;

	Vector4 lightColour[MAX_LIGHT_COUNT];
	Vector3 lightPos[MAX_LIGHT_COUNT];
	GLfloat lightRadius[MAX_LIGHT_COUNT];

	void AddLight (Vector3 pos, Vector4 colour, float radius);
	void SetMultiLights ();

	void RenderHeightMap ();

	// Particle
	VolcanoParticleEmitter* volcanoEmitter;
	SnowParticleEmitter* snowEmitter;
	Shader* particleShader;

	void SetShaderParticleSize (float f);
	void RenderParticle ();

	// Water
	Shader* reflectShader;
	Mesh* waterMesh;
	float waterRotate;

	void RenderWater ();

	// Shadow hellknight
	Shader* sceneShader;
	Shader* shadowShader;

	GLuint shadowTex;
	GLuint shadowFBO;

	MD5FileData* hellData;
	MD5Node* hellNode;
	GLuint hellTexture;
	GLuint hellBumpMap;

	Mesh* floor;

	void DrawMesh ();
	void DrawFloor ();
	void DrawShadowScene ();
	void DrawCombinedScene ();
	void RenderHellShadow ();

	int used_mem;
};