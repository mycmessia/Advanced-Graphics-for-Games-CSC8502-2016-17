#include <ctime>
#include <cstdlib>
#include "Mesh.h"

Mesh::Mesh (void)
{
	for (int i = 0; i < MAX_BUFFER; i++)
	{
		bufferObject[i] = 0;
	}
	glGenVertexArrays (1, &arrayObject);

	numVertices = 0;
	vertices = nullptr;
	colours = nullptr;
	type = GL_TRIANGLES;

	texture = 0;
	textureCoords = NULL;

	srand (static_cast <unsigned> (time (0)));
}

Mesh::~Mesh (void)
{
	glDeleteVertexArrays (1, &arrayObject);
	glDeleteBuffers (MAX_BUFFER, bufferObject);
	delete[] vertices;
	delete[] colours;

	glDeleteTextures (1, &texture);
	delete[] textureCoords;
}

Mesh* Mesh::GenerateQuad ()
{
	Mesh * m = new Mesh ();
	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];

	m->vertices[0] = Vector3 (-1.0f, -1.0f, 0.0f);
	m->vertices[1] = Vector3 (-1.0f, 1.0f, 0.0f);
	m->vertices[2] = Vector3 (1.0f, -1.0f, 0.0f);
	m->vertices[3] = Vector3 (1.0f, 1.0f, 0.0f);

	m->textureCoords[0] = Vector2 (0.0f, 1.0f);
	m->textureCoords[1] = Vector2 (0.0f, 0.0f);
	m->textureCoords[2] = Vector2 (1.0f, 1.0f);
	m->textureCoords[3] = Vector2 (1.0f, 0.0f);

	for (int i = 0; i < 4; ++i)
	{
		m->colours[i] = Vector4 (1.0f, 1.0f, 1.0f, 1.0f);
	}

	m->BufferData ();
	return m;
}

Mesh* Mesh::GenerateTriangle ()
{
	Mesh* m = new Mesh ();
	m->numVertices = 3;

	m->vertices = new Vector3[m->numVertices];
	m->vertices[0] = Vector3 (-0.9f, -0.9f, 0.0f);
	m->vertices[1] = Vector3 (0.85f, -0.9f, 0.0f);
	m->vertices[2] = Vector3 (-0.9f, 0.85f, 0.0f);

	m->colours = new Vector4[m->numVertices];
	m->colours[0] = Vector4 (0.5f, 0.0f, 0.0f, 1.0f);
	m->colours[1] = Vector4 (-0.5f, 0.5f, 0.0f, 1.0f);
	m->colours[2] = Vector4 (0.0f, 0.0f, 1.0f, 1.0f);

	m->textureCoords = new Vector2[m->numVertices];
	m->textureCoords[0] = Vector2 (0.5f, 0.0f);
	m->textureCoords[1] = Vector2 (1.0f, 1.0f);
	m->textureCoords[2] = Vector2 (0.0f, 1.0f);

	m->textureCoords2 = new Vector2[m->numVertices];
	m->textureCoords2[0] = Vector2 (0.5f, 0.0f);
	m->textureCoords2[1] = Vector2 (1.0f, 1.0f);
	m->textureCoords2[2] = Vector2 (0.0f, 1.0f);

	m->BufferData ();
	return m;
}

void Mesh::BufferData ()
{
	glBindVertexArray (arrayObject);

	glGenBuffers (1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer (GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData (GL_ARRAY_BUFFER, numVertices * sizeof (Vector4), vertices, GL_STATIC_DRAW);
	// first parameter is the layout location in vertex shader glsl #version >= 420 
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray (0);

	if (colours)
	{
		glGenBuffers (1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer (GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferData (GL_ARRAY_BUFFER, numVertices * sizeof (Vector4), colours, GL_STATIC_DRAW);
		glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray (1);
	}

	if (textureCoords)
	{
		glGenBuffers (1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer (GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferData (GL_ARRAY_BUFFER, numVertices * sizeof (Vector2), textureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray (2);
	}


	glBindVertexArray (0);
}

void Mesh::ChangeColor ()
{
	void* change = glMapBuffer (GL_ARRAY_BUFFER, GL_READ_WRITE);

	float r = static_cast <float> (rand ()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float> (rand ()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (rand ()) / static_cast <float> (RAND_MAX);

	Vector4* changed_colours = new Vector4[3];
	changed_colours[0] = Vector4 (r, g, b, 1.0f);
	changed_colours[1] = Vector4 (r, g, b, 1.0f);
	changed_colours[2] = Vector4 (r, g, b, 1.0f);

	memcpy (change, changed_colours, numVertices * sizeof (Vector4));

	delete[] changed_colours;

	glUnmapBuffer (GL_ARRAY_BUFFER);
}

void Mesh::Draw ()
{
	glBindVertexArray (arrayObject);
	glDrawArrays (type, 0, numVertices);
	glBindVertexArray (0);
}