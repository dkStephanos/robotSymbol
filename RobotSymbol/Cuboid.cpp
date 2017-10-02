#include "Cuboid.h"
#include "ObjectGenerator.h"
#include "OGLElementArrayBuffer.h"
#include "OGLArrayBuffer.h"

Cuboid::Cuboid(const string& name, float width, float depth, float height, RGBA faceColor) :
	BaseCuboid(name, width, depth, height, faceColor)
{
	this->generate();
}

Cuboid::~Cuboid()
{
}

void Cuboid::generate()
{
	ObjectGenerator gen;
	gen.clear();

	float halfWidth = this->width / 2.0f;
	float halfDepth = this->depth / 2.0f;
	float halfHeight = this->height / 2.0f;

	// Front face
	gen.positions[0] = { -halfWidth, halfHeight, halfDepth, 1.0f };
	gen.positions[1] = { -halfWidth, -halfHeight, halfDepth, 1.0f };
	gen.positions[2] = { halfWidth, -halfHeight, halfDepth, 1.0f };
	gen.positions[3] = { halfWidth, halfHeight, halfDepth, 1.0f };
	// Right face
	gen.positions[4] = { halfWidth, halfHeight, halfDepth, 1.0f };
	gen.positions[5] = { halfWidth, -halfHeight, halfDepth, 1.0f };
	gen.positions[6] = { halfWidth, -halfHeight, -halfDepth, 1.0f };
	gen.positions[7] = { halfWidth, halfHeight, -halfDepth, 1.0f };
	// Back face
	gen.positions[8] = { halfWidth, halfHeight, -halfDepth, 1.0f };
	gen.positions[9] = { halfWidth, -halfHeight, -halfDepth, 1.0f };
	gen.positions[10] = { -halfWidth, -halfHeight, -halfDepth, 1.0f };
	gen.positions[11] = { -halfWidth, halfHeight, -halfDepth, 1.0f };
	// Left face
	gen.positions[12] = { -halfWidth, halfHeight, -halfDepth, 1.0f };
	gen.positions[13] = { -halfWidth, -halfHeight, -halfDepth, 1.0f };
	gen.positions[14] = { -halfWidth, -halfHeight, halfDepth, 1.0f };
	gen.positions[15] = { -halfWidth, halfHeight, halfDepth, 1.0f };
	// Top face
	gen.positions[16] = { -halfWidth, halfHeight, -halfDepth, 1.0f };
	gen.positions[17] = { -halfWidth, halfHeight, halfDepth, 1.0f };
	gen.positions[18] = { halfWidth, halfHeight, halfDepth, 1.0f };
	gen.positions[19] = { halfWidth, halfHeight, -halfDepth, 1.0f };
	// Bottom face
	gen.positions[20] = { -halfWidth, -halfHeight, halfDepth, 1.0f };
	gen.positions[21] = { -halfWidth, -halfHeight, -halfDepth, 1.0f };
	gen.positions[22] = { halfWidth, -halfHeight, -halfDepth, 1.0f };
	gen.positions[23] = { halfWidth, -halfHeight, halfDepth, 1.0f };

	size_t numberOfVertices = gen.getNumberOfVertices();
	size_t ci = 0;
	// Colors
	for (size_t vertex = 0; vertex < numberOfVertices; vertex++) {
		gen.colors[ci++] = {
			this->faceColor.red,
			this->faceColor.green,
			this->faceColor.blue,
			this->faceColor.alpha
		};
	}

	size_t ni = 0;
	// Normals
	// Front face
	for (int vertex = 0; vertex < 4; vertex++) {
		gen.normals[ni++] = { 0, 0, 1 };
	}
	// Right face
	for (int vertex = 0; vertex < 4; vertex++) {
		gen.normals[ni++] = { 1, 0, 0 };
	}
	// Back face
	for (int vertex = 0; vertex < 4; vertex++) {
		gen.normals[ni++] = { 0, 0, -1 };
	}
	// Left face
	for (int vertex = 0; vertex < 4; vertex++) {
		gen.normals[ni++] = { -1, 0, 0 };
	}
	// Top face
	for (int vertex = 0; vertex < 4; vertex++) {
		gen.normals[ni++] = { 0, 1, 0 };
	}
	// Bottom face
	for (int vertex = 0; vertex < 4; vertex++) {
		gen.normals[ni++] = { 0, -1, 0 };
	}

	// --------------------------------------
	// The index data: 3 indices per triangle
	// --------------------------------------
	// Front face
	gen.triangles.push_back({ 0, 1, 2 });
	gen.triangles.push_back({ 0, 2, 3 });
	// Right face
	gen.triangles.push_back({ 4, 5, 6 });
	gen.triangles.push_back({ 4, 6, 7 });
	// Back face
	gen.triangles.push_back({ 8, 9, 10 });
	gen.triangles.push_back({ 8, 10, 11 });
	// Left face
	gen.triangles.push_back({ 12, 13, 14 });
	gen.triangles.push_back({ 12, 14, 15 });
	// Top face
	gen.triangles.push_back({ 16, 17, 18 });
	gen.triangles.push_back({ 16, 18, 19 });
	// Bottom face
	gen.triangles.push_back({ 20, 21, 22 });
	gen.triangles.push_back({ 20, 22, 23 });

	float* vertexData = gen.generateVertexData();
	short* indexData = gen.generateIndexData();

	this->createElementArrayPCN("Triangles", vertexData, indexData, GL_TRIANGLES);
}
