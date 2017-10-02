#include "Axis.h"
#include "ObjectGenerator.h"

Axis::Axis(const string& name, float length) :
	OGL3DObject(name)
{
	this->length = length;
	this->generate();
}

Axis::~Axis()
{
}

void Axis::generate()
{
	if (this->length <= 0) this->length = 1;

	ObjectGenerator gen;
	
	gen.clear();

	// X-Axis
	gen.positions[0] = { 0, 0, 0, 1 };
	gen.positions[1] = { length, 0, 0, 1 };

	gen.colors[0] = { 1, 0, 0, 1 };
	gen.colors[1] = { 1, 0, 0, 1 };

	// Y-Axis
	gen.positions[2] = { 0, 0, 0, 1 };
	gen.positions[3] = { 0, length, 0, 1 };

	gen.colors[2] = { 0, 1, 0, 1 };
	gen.colors[3] = { 0, 1, 0, 1 };

	// Z-Axis
	gen.positions[4] = { 0, 0, 0, 1 };
	gen.positions[5] = { 0, 0, length, 1 };

	gen.colors[4] = { 0, 0, 1, 1 };
	gen.colors[5] = { 0, 0, 1, 1 };

	// Indexes
	gen.lines.push_back({ 0, 1 });
	gen.lines.push_back({ 2, 3 });
	gen.lines.push_back({ 4, 5 });

	float* vertexData = gen.generateVertexData();
	short* indexData = gen.generateIndexData();
	
	this->createElementArrayPC("Axes", vertexData, indexData, GL_LINES);
}
