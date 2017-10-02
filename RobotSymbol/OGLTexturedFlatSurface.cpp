#include "OGLTexturedFlatSurface.h"
#include "ObjectGenerator.h"
#include "MathUtil.h"

OGLTexturedFlatSurface::OGLTexturedFlatSurface(
	const string& name,
	float width, float depth,
	unsigned int numberOfWidthSegments, unsigned int numberOfDepthSegments,
	RGBA surfaceColor) : OGL3DObject(name)
{
	this->width = width;
	this->depth = depth;
	this->numberOfDepthSegments = numberOfDepthSegments;
	this->numberOfWidthSegments = numberOfWidthSegments;
	this->surfaceColor = surfaceColor;
	this->generate();
}

OGLTexturedFlatSurface::~OGLTexturedFlatSurface()
{
}

void OGLTexturedFlatSurface::generate()
{
//	ElementArray arr = ObjectGenerator::generateTextureFlatSurface(
//		this->numberOfWidthSegments, 
//		this->numberOfDepthSegments, 
//		this->width, 
//		this->depth,
//		this->surfaceColor
//	);
	ObjectGenerator gen;
	gen.clear();

	float xPos = -this->width / 2;
	float zPos = -this->depth / 2;
	float segmentWidth = this->width / this->numberOfWidthSegments;
	float segmentDepth = this->depth / this->numberOfDepthSegments;
	size_t A = 0, B, C, D;
	for (size_t row = 0; row < this->numberOfDepthSegments; row++) {
		for (size_t col = 0; col < this->numberOfWidthSegments; col++) {
			// We need 4 vertices per segment because some vertices need
			// 2 sets of texture coordinates.
			// A
			gen.positions[A] = { xPos, 0.0f, zPos, 1.0f };
			gen.colors[A] = { 
				surfaceColor.red, surfaceColor.green, surfaceColor.blue, 1.0f };
			gen.normals[A] = { 0, 1, 0 };
			gen.textureCoords[A] = { 0, 1 };
			// B
			B = A + 1;
			gen.positions[B] = { xPos, 0.0f, zPos + segmentDepth, 1.0f };
			gen.colors[B] = { 
				surfaceColor.red, surfaceColor.green, surfaceColor.blue, 1.0f };
			gen.normals[B] = { 0, 1, 0 };
			gen.textureCoords[B] = { 0, 0 };
			// C
			C = A + 2;
			gen.positions[C] = { 
				xPos + segmentWidth, 0.0f, zPos + segmentDepth, 1.0f };
			gen.colors[C] = { 
				surfaceColor.red, surfaceColor.green, surfaceColor.blue, 1.0f };
			gen.normals[C] = { 0, 1, 0 };
			gen.textureCoords[C] = { 1, 0 };
			// D
			D = A + 3;
			gen.positions[D] = { xPos + segmentWidth, 0.0f, zPos, 1.0f };
			gen.colors[D] = { 
				surfaceColor.red, surfaceColor.green, surfaceColor.blue, 1.0f };
			gen.normals[D] = { 0, 1, 0 };
			gen.textureCoords[D] = { 1, 1 };
			// A--D
			// |\ |
			// | \|
			// B--C
			gen.triangles.push_back({ A, B, C });
			gen.triangles.push_back({ A, C, D });
			
			xPos += segmentWidth;
			A += 4; // 4 vertices per segment
		}
		xPos = -this->width / 2;
		zPos += segmentDepth;
	}

	float* vertexData = gen.generateVertexData();
	short* indexData = gen.generateIndexData();

	this->createElementArrayPCNT("Triangles", vertexData, indexData, GL_TRIANGLES);
}
