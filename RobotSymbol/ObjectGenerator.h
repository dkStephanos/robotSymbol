#pragma once
#ifndef OBJECT_GENERATOR
#define OBJECT_GENERATOR

#include "RGBA.h"
#include "TextFileReader.h"

#include <map>
using std::map;
#include <vector>
using std::vector;

class OGL3DObject;

struct ElementArray {
	float *vertexData;
	short *indexData;
};

struct Position {
	float x, y, z, w;
};

//struct Color {
//	float r, g, b, a;
//};

struct Normal {
	float x, y, z;
};

struct Tex {
	float s, t;
};

struct Triangle {
	unsigned int idx1, idx2, idx3;
};

struct Line {
	unsigned int idx1, idx2;
};

class ObjectGenerator
{
public:
	map<unsigned int, Position> positions;
	map<unsigned int, Color> colors;
	map<unsigned int, Normal> normals;
	map<unsigned int, Tex> textureCoords;
	vector<Triangle> triangles;
	vector<Line> lines;

public:
	ObjectGenerator();
	~ObjectGenerator();

public:
	void clear();

	float* generateVertexData();

	short* generateIndexData();

	unsigned int getNumberOfVertices();

	void addTriangleAndGenerateNormals(const Triangle& triIdx);

	

protected:
	unsigned int getSizeOfVextexData();

	unsigned int getSizeOfIndexData();

	unsigned int getComponentCount();

	

};

#endif

