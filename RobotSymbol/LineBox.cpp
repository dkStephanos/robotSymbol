#include "LineBox.h"
#include "ObjectGenerator.h"
#include "OGLElementArrayBuffer.h"
#include "OGLArrayBuffer.h"

LineBox::LineBox(const string& name, float width, float depth, float height, RGBA lineColor) :
	BaseCuboid(name, width, depth, height, lineColor)
{
	this->generate();
}

LineBox::~LineBox()
{
}

void LineBox::generate()
{
	float halfWidth = this->width / 2.0f;
	float halfDepth = this->depth / 2.0f;
	float halfHeight = this->height / 2.0f;

	ObjectGenerator gen;
	gen.clear();

	// Front face
	gen.positions[0] = { -halfWidth, halfHeight, halfDepth, 1.0f };
	gen.positions[1] = { -halfWidth, -halfHeight, halfDepth, 1.0f };
	gen.positions[2] = { halfWidth, -halfHeight, halfDepth, 1.0f };
	gen.positions[3] = { halfWidth, halfHeight, halfDepth, 1.0f };
	// Back face
	gen.positions[4] = { halfWidth, halfHeight, -halfDepth, 1.0f };
	gen.positions[5] = { halfWidth, -halfHeight, -halfDepth, 1.0f };
	gen.positions[6] = { -halfWidth, -halfHeight, -halfDepth, 1.0f };
	gen.positions[7] = { -halfWidth, halfHeight, -halfDepth, 1.0f };

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

	gen.lines.push_back({ 0, 1 });
	gen.lines.push_back({ 1, 2 });
	gen.lines.push_back({ 2, 3 });
	gen.lines.push_back({ 3, 0 });
	gen.lines.push_back({ 4, 5 });
	gen.lines.push_back({ 5, 6 });
	gen.lines.push_back({ 6, 7 });
	gen.lines.push_back({ 7, 4 });
	gen.lines.push_back({ 0, 7 });
	gen.lines.push_back({ 3, 4 });
	gen.lines.push_back({ 2, 5 });
	gen.lines.push_back({ 1, 6 });

	float* vertexData = gen.generateVertexData();
	short* indexData = gen.generateIndexData();
	this->createElementArrayPC("Lines", vertexData, indexData, GL_LINES);
}

