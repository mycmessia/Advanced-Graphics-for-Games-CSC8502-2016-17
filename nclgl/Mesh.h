#pragma once
#include "OGLRenderer.h"

enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, MAX_BUFFER
};

class Mesh {
public:
	Mesh (void);
	~Mesh (void);

	virtual void Draw ();
	void ChangeColor ();
	static Mesh* GenerateTriangle ();

	void SetTexture (GLuint tex) { texture = tex; }
	GLuint GetTexture () { return texture; }

protected:
	GLuint texture;
	Vector2* textureCoords;

	void BufferData ();

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;

	Vector3* vertices;
	Vector4* colours;
};