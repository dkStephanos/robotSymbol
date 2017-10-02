#include "Plane.h"
#include "OGLElementArrayBuffer.h"
#include "OGLArrayBuffer.h"

Plane::Plane(float height, float width) : OGL3DObject("Plane")
{
	this->height = height;
	this->width = width;
	this->generate();
}

Plane::~Plane()
{
}

void Plane::generate()
{
	//this->setIndexedArrayType();

	const int NumberOfTriangles = 2;
	const int NumberOfVertices = NumberOfTriangles * 3;
	const int SizeOfVertexData = NumberOfVertices * 13;
	float* vertexData = new float[SizeOfVertexData + 1];
	vertexData[0] = SizeOfVertexData;
	int i = 1;
	// A---D
	// |\  |
	// | \ |
	// |  \|
	// B---C
	float halfHeight = this->height / 2.0f;
	float halfWidth = this->width / 2.0f;
	// A positions
	vertexData[i++] = -halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = 0.0f;
	vertexData[i++] = 1.0f;
	// B positions
	vertexData[i++] = -halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = 0.0f;
	vertexData[i++] = 1.0f;
	// C positions
	vertexData[i++] = halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = 0.0f;
	vertexData[i++] = 1.0f;
	// A positions
	vertexData[i++] = -halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = 0.0f;
	vertexData[i++] = 1.0f;
	// C positions
	vertexData[i++] = halfWidth;
	vertexData[i++] = -halfHeight;
	vertexData[i++] = 0.0f;
	vertexData[i++] = 1.0f;
	// D positions
	vertexData[i++] = halfWidth;
	vertexData[i++] = halfHeight;
	vertexData[i++] = 0.0f;
	vertexData[i++] = 1.0f;
	// Colors
	for (int count = 0; count < NumberOfVertices; count++) {
		vertexData[i++] = 1.0f;
		vertexData[i++] = 1.0f;
		vertexData[i++] = 1.0f;
		vertexData[i++] = 1.0f;
	}
	// Normals
	for (int count = 0; count < NumberOfVertices; count++) {
		vertexData[i++] = 0.0f;
		vertexData[i++] = 0.0f;
		vertexData[i++] = 1.0f;
	}
	// Texture Coordinates
	// A
	vertexData[i++] = 0.0f;
	vertexData[i++] = 1.0f;
	// B
	vertexData[i++] = 0.0f;
	vertexData[i++] = 0.0f;
	// C
	vertexData[i++] = 1.0f;
	vertexData[i++] = 0.0f;
	// A
	vertexData[i++] = 0.0f;
	vertexData[i++] = 1.0f;
	// C
	vertexData[i++] = 1.0f;
	vertexData[i++] = 0.0f;
	// D
	vertexData[i++] = 1.0f;
	vertexData[i++] = 1.0f;
	//this->setVertexData(vertexData);

	const int NumberOfIndexes = NumberOfVertices;
	short* indexData = new short[NumberOfIndexes + 1];
	indexData[0] = NumberOfIndexes;
	for (int count = 0; count < NumberOfIndexes; count++) {
		indexData[count+1] = count;
	}
	//this->setIndexData(indexData);

	//VBOObject* vboObject = OGLObject::createTextureVBOObject(
	//	"Triangles", vertexData, indexData, GL_TRIANGLES);
	//this->sendBuffersToGPU(vboObject);
	this->createElementArrayPCNT("Triangles", vertexData, indexData, GL_TRIANGLES);
}
