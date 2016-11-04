#pragma once
#include "../../NCLGL/OGLRenderer.h"

# pragma once

class Renderer : public OGLRenderer {
public :
	Renderer (Window& parent);
	virtual ~Renderer (void);

	virtual void RenderScene ();

	void ToggleObject ();
	void ToggleDepth ();
	void ToggleAlphaBlend ();
	void ToggleBlendMode ();
	void MoveObject (float by);

protected :
	Mesh* meshes[2];
	Vector3 positions[2];

	bool modifyObject;
	bool usingDepth;
	bool usingAlpha;
	int blendMode;
};
