#pragma once
#include "../../NCLGL/OGLRenderer.h"

class Renderer : public OGLRenderer {
public:
	Renderer (Window &parent);
	virtual ~Renderer (void);

	virtual void RenderScene ();

	void UpdateTextureMatrix (float rotation);
	void ToggleRepeating ();
	void ToggleFiltering ();

protected:
	Mesh* triangle;

	bool filtering;
	bool repeating;
};