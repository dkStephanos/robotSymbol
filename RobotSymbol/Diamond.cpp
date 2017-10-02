#include "Diamond.h"
#include "ObjectGenerator.h"
#include "MathUtil.h"

Diamond::Diamond(const string& name, float scaler) : OGL3DObject(name)
{
	this->scaler = scaler;
	this->generate();
}

Diamond::~Diamond()
{
}

void Diamond::generate()
{
	ObjectGenerator gen;
	gen.clear();

	// Positions {x, y, z, w}
	gen.positions[0] = { -1, 2, -1, 1 };
	gen.positions[1] = { -1, 2, 1, 1 };
	gen.positions[2] = { 1, 2, -1, 1 };
	gen.addTriangleAndGenerateNormals({ 0, 1, 2 });

	gen.positions[3] = { 1, 2, -1, 1 };
	gen.positions[4] = { -1, 2, 1, 1 };
	gen.positions[5] = { 1, 2, 1, 1 };
	gen.addTriangleAndGenerateNormals({ 3, 4, 5 });

	gen.positions[6] = { -1, 2, 1, 1 };
	gen.positions[7] = { -2, 1, 1, 1 };
	gen.positions[8] = { -1, 1, 2, 1 };
	gen.addTriangleAndGenerateNormals({ 6, 7, 8 });

	gen.positions[9] = { -1, 2, 1, 1 };
	gen.positions[10] = { -1, 1, 2, 1 };
	gen.positions[11] = { 0, 2, 1, 1 };
	gen.addTriangleAndGenerateNormals({ 9, 10, 11 });

	gen.positions[12] = { 0, 2, 1, 1 };
	gen.positions[13] = { -1, 1, 2, 1 };
	gen.positions[14] = { 1, 1, 2, 1 };
	gen.addTriangleAndGenerateNormals({ 12, 13, 14 });

	gen.positions[15] = { 1, 2, 1, 1 };
	gen.positions[16] = { 0, 2, 1, 1 };
	gen.positions[17] = { 1, 1, 2, 1 };
	gen.addTriangleAndGenerateNormals({ 15, 16, 17 });

	gen.positions[18] = { 1, 2, 0, 1 };
	gen.positions[19] = { 1, 2, 1, 1 };
	gen.positions[20] = { 2, 1, 1, 1 };
	gen.addTriangleAndGenerateNormals({ 18, 19, 20 });

	gen.positions[21] = { 1, 2, 0, 1 };
	gen.positions[22] = { 2, 1, 1, 1 };
	gen.positions[23] = { 2, 1, -1, 1 };
	gen.addTriangleAndGenerateNormals({ 21, 22, 23 });

	gen.positions[24] = { 1, 2, -1, 1 };
	gen.positions[25] = { 1, 2, 0, 1 };
	gen.positions[26] = { 2, 1, -1, 1 };
	gen.addTriangleAndGenerateNormals({ 24, 25, 26 });

	gen.positions[27] = { 1, 2, -1, 1 };
	gen.positions[28] = { 2, 1, -1, 1 };
	gen.positions[29] = { 1, 1, -2, 1 };
	gen.addTriangleAndGenerateNormals({ 27, 28, 29 });

	gen.positions[30] = { 0, 2, -1, 1 };
	gen.positions[31] = { 1, 2, -1, 1 };
	gen.positions[32] = { 1, 1, -2, 1 };
	gen.addTriangleAndGenerateNormals({ 30, 31, 32 });

	gen.positions[33] = { 0, 2, -1, 1 };
	gen.positions[34] = { 1, 1, -2, 1 };
	gen.positions[35] = { -1, 1, -2, 1 };
	gen.addTriangleAndGenerateNormals({ 33, 34, 35 });

	gen.positions[36] = { -1, 2, -1, 1 };
	gen.positions[37] = { 0, 2, -1, 1 };
	gen.positions[38] = { -1, 1, -2, 1 };
	gen.addTriangleAndGenerateNormals({ 36, 37, 38 });

	gen.positions[39] = { -1, 2, -1, 1 };
	gen.positions[40] = { -1, 1, -2, 1 };
	gen.positions[41] = { -2, 1, -1, 1 };
	gen.addTriangleAndGenerateNormals({ 39, 40, 41 });

	gen.positions[42] = { -1, 2, -1, 1 };
	gen.positions[43] = { -2, 1, -1, 1 };
	gen.positions[44] = { -1, 2, 0, 1 };
	gen.addTriangleAndGenerateNormals({ 42, 43, 44 });

	gen.positions[45] = { -1, 2, 0, 1 };
	gen.positions[46] = { -2, 1, -1, 1 };
	gen.positions[47] = { -2, 1, 1, 1 };
	gen.addTriangleAndGenerateNormals({ 45, 46, 47 });

	gen.positions[48] = { -1, 2, 0, 1 };
	gen.positions[49] = { -2, 1, 1, 1 };
	gen.positions[50] = { -1, 2, 1, 1 };
	gen.addTriangleAndGenerateNormals({ 48, 49, 50 });

	gen.positions[51] = { 1, 2, 1, 1 };
	gen.positions[52] = { 1, 1, 2, 1 };
	gen.positions[53] = { 2, 1, 1, 1 };
	gen.addTriangleAndGenerateNormals({ 51, 52, 53 });

	gen.positions[54] = { 1, 1, 2, 1 };
	gen.positions[55] = { -1, 1, 2, 1 };
	gen.positions[56] = { 0, -2, 0, 1 };
	gen.addTriangleAndGenerateNormals({ 54, 55, 56 });

	gen.positions[57] = { -1, 1, 2, 1 };
	gen.positions[58] = { -2, 1, 1, 1 };
	gen.positions[59] = { 0, -2, 0, 1 };
	gen.addTriangleAndGenerateNormals({ 57, 58, 59 });

	gen.positions[60] = { 2, 1, 1, 1 };
	gen.positions[61] = { 1, 1, 2, 1 };
	gen.positions[62] = { 0, -2, 0, 1 };
	gen.addTriangleAndGenerateNormals({ 60, 61, 62 });

	gen.positions[63] = { 0, -2, 0, 1 };
	gen.positions[64] = { -1, 1, -2, 1 };
	gen.positions[65] = { 1, 1, -2, 1 };
	gen.addTriangleAndGenerateNormals({ 63, 64, 65 });

	gen.positions[66] = { -1, 1, -2, 1 };
	gen.positions[67] = { 0, -2, 0, 1 };
	gen.positions[68] = { -2, 1, -1, 1 };
	gen.addTriangleAndGenerateNormals({ 66, 67, 68 });

	gen.positions[69] = { 2, 1, -1, 1 };
	gen.positions[70] = { 0, -2, 0, 1 };
	gen.positions[71] = { 1, 1, -2, 1 };
	gen.addTriangleAndGenerateNormals({ 69, 70, 71 });
	
	gen.positions[72] = { -2, 1, 1, 1 };
	gen.positions[73] = { -2, 1, -1, 1 };
	gen.positions[74] = { 0, -2, 0, 1 };
	gen.addTriangleAndGenerateNormals({ 72, 73, 74 });

	gen.positions[75] = { 2, 1, 1, 1 };
	gen.positions[76] = { 0, -2, 0, 1 };
	gen.positions[77] = { 2, 1, -1, 1 };
	gen.addTriangleAndGenerateNormals({ 75, 76, 77 });

	// Scales our positions by the scaler float
	for (int i = 0; i < 78; i++)
	{
		gen.positions[i].x *= this->scaler;
		gen.positions[i].y *= this->scaler;
		gen.positions[i].z *= this->scaler;
	}

	// Colors {r, g, b, a}
	for (int i = 0; i < 78; i++)
	{
		gen.colors[i] = { .185f, .242f, .255f, 1 };
	}
	// Texture Coordinates {s, t}
	size_t ti = 0;
	for (int i = 0; i < 78; i++)
	{
		gen.textureCoords[ti++] = { 0.0f, 0.0f };
		gen.textureCoords[ti++] = { 0.1f, 0.0f };
		gen.textureCoords[ti++] = { 0.5f, 1.0f };
	}
	
	float* vertexData = gen.generateVertexData();
	short* indexData = gen.generateIndexData();

	this->createElementArrayPCNT("Triangles", vertexData, indexData, GL_TRIANGLES);
}
