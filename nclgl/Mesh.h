#pragma once
#include "OGLRenderer.h"

enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, 
	NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER, 
	MAX_BUFFER
};

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

	void SetBumpMap (GLuint tex) { bumpTexture = tex; }
	GLuint GetBumpMap () { return bumpTexture; }

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

	GLuint numIndices;
	unsigned int* indices;

	void GenerateNormals ();
	Vector3* normals;

	void GenerateTangents ();
	Vector3 GenerateTangent (const Vector3& a, const Vector3& b,
							 const Vector3& c, const Vector2& ta,
							 const Vector2& tb, const Vector2& tc);
	
	Vector3* tangents;
	GLuint bumpTexture;
};