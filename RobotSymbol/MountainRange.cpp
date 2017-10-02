#include "MountainRange.h"
#include "IBehavior.h"
#include "TextFileReader.h"
#include "ObjectGenerator.h"
#include <fstream>
#include <iostream>
using std::ifstream;
using std::ofstream;
using std::getline;
using std::cout;
using std::endl;

/// <summary>
/// Initializes a new instance of the <see cref="MountainRange"/> class.
/// </summary>
/// <param name="name">The name.</param>
MountainRange::MountainRange(const string& name, float scaler) :
	OGL3DObject(name) 
{
	this->generate(scaler);
}

/// <summary>
/// Updates the specified elapsed time ms.
/// </summary>
/// <param name="elapsedTimeMS">The elapsed time ms.</param>
void MountainRange::update(float elapsedTimeMS)
{
	if (this->behavior) {
		this->behavior->update(elapsedTimeMS);
	}
}

/// <summary>
/// Finalizes an instance of the <see cref="MountainRange"/> class.
/// </summary>
MountainRange::~MountainRange()
{
}

void MountainRange::generate(float scaler)
{
	TextFileReader *reader = new TextFileReader();
	ObjectGenerator gen;
	gen.clear();

	string line, x1, x2, x3, y1, y2, y3,
		z1, z2, z3, w1, w2, w3,
		r1, r2, r3, g1, g2, g3,
		b1, b2, b3, a1, a2, a3;
	ifstream fin("MountainRange.txt");

	unsigned int pos = 0, col = 0;
	for (int i = 0; i < 78; i++)
	{
		//gets the vertex number
		getline(fin, line);
		//gets the position data for the first vertex
		getline(fin, x1);
		getline(fin, y1);
		getline(fin, z1);
		getline(fin, w1);
		//gets the color data for the first vertex
		getline(fin, r1);
		getline(fin, g1);
		getline(fin, b1);
		getline(fin, a1);
		//gets the vertex number
		getline(fin, line);
		//gets the position data for the second vertex
		getline(fin, x2);
		getline(fin, y2);
		getline(fin, z2);
		getline(fin, w2);
		//gets the color data for the second vertex
		getline(fin, r2);
		getline(fin, g2);
		getline(fin, b2);
		getline(fin, a2);
		//gets the vertex number
		getline(fin, line);
		//gets the position data for the third vertex
		getline(fin, x3);
		getline(fin, y3);
		getline(fin, z3);
		getline(fin, w3);
		//gets the color data for the third vertex
		getline(fin, r3);
		getline(fin, g3);
		getline(fin, b3);
		getline(fin, a3);

		

		gen.positions[pos++] = { (std::stof(x1) * scaler), (std::stof(y1) * scaler), (std::stof(z1) * scaler), std::stof(w1) };
		gen.positions[pos++] = { (std::stof(x2) * scaler), (std::stof(y2) * scaler), (std::stof(z2) * scaler), std::stof(w2) };
		gen.positions[pos++] = { (std::stof(x3) * scaler), (std::stof(y3) * scaler), (std::stof(z3) * scaler), std::stof(w3) };
		gen.addTriangleAndGenerateNormals({ (pos - 3), (pos - 2), (pos - 1) });
		gen.colors[col++] = { std::stof(r1), std::stof(g1), std::stof(b1), std::stof(a1) };
		gen.colors[col++] = { std::stof(r2), std::stof(g2), std::stof(b2), std::stof(a2) };
		gen.colors[col++] = { std::stof(r3), std::stof(g3), std::stof(b3), std::stof(a3) };
	}

	float* vertexData = gen.generateVertexData();
	short* indexData = gen.generateIndexData();

	this->createElementArrayPCN("Triangles", vertexData, indexData, GL_TRIANGLES);
}
