#pragma once
#include "OGLRenderer.h"

#define MAX_BUFFER 3

class Mesh {
public:
	Mesh (void);
	~Mesh (void);

	virtual void Draw ();
	void ChangeColor ();
	static Mesh* GenerateTriangle (unsigned tutorial);
	static Mesh* GenerateQuad (unsigned tutorial);

	void SetTexture (GLuint tex) { texture = tex; }
	GLuint GetTexture () { return texture; }

	void SetTexture2 (GLuint tex) { texture2 = tex; }
	GLuint GetTexture2 () { return texture2; }

protected:
	GLuint texture;
	GLuint texture2;
	Vector2* textureCoords;
	Vector2* textureCoords2;

	void BufferData ();

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;

	Vector3* vertices;
	Vector4* colours;
};