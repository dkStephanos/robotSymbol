#include "ObjectGenerator.h"
#include "MathUtil.h"
#include "OGL3DObject.h"

ObjectGenerator::ObjectGenerator()
{
}

ObjectGenerator::~ObjectGenerator()
{
}

void ObjectGenerator::clear()
{
	this->positions.clear();
	this->colors.clear();
	this->normals.clear();
	this->textureCoords.clear();
	this->triangles.clear();
	this->lines.clear();
}

float* ObjectGenerator::generateVertexData()
{
	unsigned int sizeOfVertexData = this->getSizeOfVextexData();
	float* vertexData = new float[sizeOfVertexData + 1];
	vertexData[0] = (float)sizeOfVertexData;

	unsigned int positionComponentCount = sizeof(Position) / sizeof(float);
	unsigned int colorComponentCount = sizeof(Color) / sizeof(float);
	unsigned int normalComponentCount = sizeof(Normal) / sizeof(float);
	unsigned int textureComponentCount = sizeof(Tex) / sizeof(float);
	unsigned int numberOfVertices = this->getNumberOfVertices();
	unsigned int pi = 1;
	unsigned int ci = (positionComponentCount * numberOfVertices) + 1;
	unsigned int ni = ((positionComponentCount + colorComponentCount) * numberOfVertices) + 1;
	unsigned int ti = ((positionComponentCount + colorComponentCount + normalComponentCount)  
		* numberOfVertices) + 1;
	for (size_t i = 0; i < numberOfVertices; i++) {
		if (this->positions.size() > 0) {
			vertexData[pi++] = this->positions[i].x;
			vertexData[pi++] = this->positions[i].y;
			vertexData[pi++] = this->positions[i].z;
			vertexData[pi++] = this->positions[i].w;
		}
		if (this->colors.size() > 0) {
			vertexData[ci++] = this->colors[i].r;
			vertexData[ci++] = this->colors[i].g;
			vertexData[ci++] = this->colors[i].b;
			vertexData[ci++] = this->colors[i].a;
		}
		if (this->normals.size() > 0) {
			vertexData[ni++] = this->normals[i].x;
			vertexData[ni++] = this->normals[i].y;
			vertexData[ni++] = this->normals[i].z;
		}
		if (this->textureCoords.size() > 0) {
			vertexData[ti++] = this->textureCoords[i].s;
			vertexData[ti++] = this->textureCoords[i].t;
		}
	}
	return vertexData;
}

short* ObjectGenerator::generateIndexData()
{
	unsigned int sizeOfIndexData = this->getSizeOfIndexData();
	short* indexData = new short[sizeOfIndexData + 1];
	indexData[0] = (short)sizeOfIndexData;

	unsigned int count = 0;
	if (this->lines.size() > 0) count = this->lines.size();
	else if (this->triangles.size() > 0) count = this->triangles.size();
	unsigned int idx = 1;
	for (size_t i = 0; i < count; i++) {
		if (this->lines.size() > 0) {
			indexData[idx++] = this->lines[i].idx1;
			indexData[idx++] = this->lines[i].idx2;
		}
		else if (this->triangles.size() > 0) {
			indexData[idx++] = this->triangles[i].idx1;
			indexData[idx++] = this->triangles[i].idx2;
			indexData[idx++] = this->triangles[i].idx3;
		}
	}
	return indexData;
}

unsigned int ObjectGenerator::getSizeOfVextexData()
{
	unsigned int numberOfElements = 0;
	numberOfElements += this->positions.size() * (sizeof(Position) / sizeof(float));
	numberOfElements += this->colors.size() * (sizeof(Color) / sizeof(float));
	numberOfElements += this->normals.size() * (sizeof(Normal) / sizeof(float));
	numberOfElements += this->textureCoords.size() * (sizeof(Tex) / sizeof(float));
	return numberOfElements;
}

unsigned int ObjectGenerator::getSizeOfIndexData()
{
	unsigned int numberOfElements = 0;
	if (this->lines.size() > 0) {
		numberOfElements = this->lines.size() * (sizeof(Line) / sizeof(unsigned int));
	}
	else if (this->triangles.size() > 0) {
		numberOfElements = this->triangles.size() * (sizeof(Triangle) / sizeof(unsigned int));
	}
	return numberOfElements;
}

unsigned int ObjectGenerator::getComponentCount()
{
	unsigned int componentCount = 0;
	if (this->positions.size() > 0) {
		componentCount += (sizeof(Position) / sizeof(float));
	}
	if (this->colors.size() > 0) {
		componentCount += (sizeof(Color) / sizeof(float));
	}
	if (this->normals.size() > 0) {
		componentCount += (sizeof(Normal) / sizeof(float));
	}
	if (this->textureCoords.size() > 0) {
		componentCount += (sizeof(Tex) / sizeof(float));
	}
	return componentCount;
}

unsigned int ObjectGenerator::getNumberOfVertices()
{
	return this->getSizeOfVextexData() / this->getComponentCount();
}

void ObjectGenerator::addTriangleAndGenerateNormals(const Triangle& triIdx)
{
	this->triangles.push_back(triIdx);
	unsigned int iA = triIdx.idx1;
	unsigned int iB = triIdx.idx2;
	unsigned int iC = triIdx.idx3;
	glm::vec3 A = glm::vec3(this->positions[iA].x, this->positions[iA].y, this->positions[iA].z);
	glm::vec3 B = glm::vec3(this->positions[iB].x, this->positions[iB].y, this->positions[iB].z);
	glm::vec3 C = glm::vec3(this->positions[iC].x, this->positions[iC].y, this->positions[iC].z);
	glm::vec3 BC = C - B;
	glm::vec3 BA = A - B;
	glm::vec3 N = glm::normalize(glm::cross(BC, BA));
	this->normals[iA] = { N.x, N.y, N.z };
	this->normals[iB] = { N.x, N.y, N.z };
	this->normals[iC] = { N.x, N.y, N.z };
}
