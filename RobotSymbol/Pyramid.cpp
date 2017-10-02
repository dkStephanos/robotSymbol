#include "Pyramid.h"
#include "ObjectGenerator.h"

Pyramid::Pyramid(const string& name, float width, float depth, float height, RGBA color) :
	OGL3DObject(name)
{
	this->width = width;
	this->depth = depth;
	this->height = height;
	this->color = color;
	this->generate();
}

Pyramid::~Pyramid()
{
}

void Pyramid::generate()
{
	float halfWidth = this->width / 2.0f;
	float halfDepth = this->depth / 2.0f;
	float halfHeight = this->height / 2.0f;

	ObjectGenerator gen;
	gen.clear();

	// South face
	gen.positions[0] = { -halfWidth, -halfHeight, halfDepth, 1.0f};
	gen.positions[1] = { halfWidth, -halfHeight, halfDepth, 1.0f };
	gen.positions[2] = { 0.0f, halfHeight, 0.0f, 1.0f };
	gen.addTriangleAndGenerateNormals({ 0, 1, 2 });
	// East face
	gen.positions[3] = { halfWidth, -halfHeight, halfDepth, 1.0f };
	gen.positions[4] = { halfWidth, -halfHeight, -halfDepth, 1.0f };
	gen.positions[5] = { 0.0f, halfHeight, 0.0f, 1.0f };
	gen.addTriangleAndGenerateNormals({ 3, 4, 5 });
	// North face
	gen.positions[6] = { halfWidth, -halfHeight, -halfDepth, 1.0f };
	gen.positions[7] = { -halfWidth, -halfHeight, -halfDepth, 1.0f };
	gen.positions[8] = { 0.0f, halfHeight, 0.0f, 1.0f };
	gen.addTriangleAndGenerateNormals({ 6, 7, 8 });
	// West face
	gen.positions[9] = { -halfWidth, -halfHeight, -halfDepth, 1.0f };
	gen.positions[10] = { -halfWidth, -halfHeight, halfDepth, 1.0f };
	gen.positions[11] = { 0.0f, halfHeight, 0.0f, 1.0f };
	gen.addTriangleAndGenerateNormals({ 9, 10, 11 });
	// Base
	// UL
	gen.positions[12] = { -halfWidth, -halfHeight, halfDepth, 1.0f };
	// LL
	gen.positions[13] = { -halfWidth, -halfHeight, -halfDepth, 1.0f };
	// LR
	gen.positions[14] = { halfWidth, -halfHeight, -halfDepth, 1.0f };
	gen.addTriangleAndGenerateNormals({ 12, 13, 14 });
	// UL
	gen.positions[15] = { -halfWidth, -halfHeight, halfDepth, 1.0f };
	// LR
	gen.positions[16] = { halfWidth, -halfHeight, -halfDepth, 1.0f };
	// UR
	gen.positions[17] = { halfWidth, -halfHeight, halfDepth, 1.0f };
	gen.addTriangleAndGenerateNormals({ 15, 16, 17 });

	// Colors
	size_t ci = 0;
	size_t numberOfVertices = gen.getNumberOfVertices();
	for (size_t vertex = 0; vertex < numberOfVertices; vertex++) {
		gen.colors[ci++] = { 
			this->color.red, 
			this->color.green, 
			this->color.blue, 
			this->color.alpha 
		};
	}

	// The faces
	size_t ti = 0;
	for (size_t face = 0; face < 4; face++) {
		gen.textureCoords[ti++] = { 0, 0 };
		gen.textureCoords[ti++] = { 1, 0 };
		gen.textureCoords[ti++] = { 0.5f, 1 };
	}
	// The base
	gen.textureCoords[12] = { 0, 1 };
	gen.textureCoords[13] = { 0, 0 };
	gen.textureCoords[14] = { 1, 0 };
	gen.textureCoords[15] = { 0, 1 };
	gen.textureCoords[16] = { 1, 0 };
	gen.textureCoords[17] = { 1, 1 };

	float* vertexData = gen.generateVertexData();
	short* indexData = gen.generateIndexData();

	this->createElementArrayPCNT("Triangles", vertexData, indexData, GL_TRIANGLES);
}
