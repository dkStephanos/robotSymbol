#include "OGLObject.h"
#include "OGL2DTexture.h"
#include "LineBox.h"
#include "OGLArrayBuffer.h"
#include "OGLElementArrayBuffer.h"

#include <glm\gtc\type_ptr.hpp>

OGLObject::OGLObject(string name) : OGLGameObject(name)
{
	this->vao = 0;
}

OGLObject::~OGLObject()
{
	auto it = this->buffers.begin();
	while (it != this->buffers.end()) {
		delete it->second;
		it++;
	}
	glDeleteVertexArrays(1, &this->vao);
}

void OGLObject::setTexture(Texture* texture)
{
	this->texture = texture;
	auto it = this->buffers.begin();
	while (it != this->buffers.end()) {
		it->second->setTexture(texture);
		it++;
	}
}

void OGLObject::setShaderProgram(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
	auto it = this->buffers.begin();
	while (it != this->buffers.end()) {
		it->second->setShaderHandle(shaderProgram);
		it++;
	}
}

void OGLObject::sendBuffersToGPU()
{
	glBindVertexArray(this->vao);
	auto it = this->buffers.begin();
	while (it != this->buffers.end()) {
		it->second->sendBufferToGPU();
		it++;
	}
	glBindVertexArray(0);
}

void OGLObject::update(float elapsedSeconds)
{
	GameObject::update(elapsedSeconds);
	this->boundingBox.referenceFrame = this->referenceFrame;
	this->boundingBox.applyTransform();
	if (this->showBoundingBox) {
		if (this->lineBox) {
			this->lineBox->setVisibility(this->visible);
			this->lineBox->referenceFrame = this->boundingBox.referenceFrame;
		}
	}
}

void OGLObject::createArrayBuffer(string name, float* vertexData, GLuint numberOfElements, GLenum primitiveType)
{
	if (this->vao == 0) {
		glGenVertexArrays(1, &this->vao);
	}
	glBindVertexArray(this->vao);
	OGLArrayBuffer* buffer = new OGLArrayBuffer();
	buffer->set(name, vertexData, primitiveType);
	buffer->generateBuffer();
	this->buffers[name] = buffer;
	glBindVertexArray(0);
}

OGLElementArrayBuffer* OGLObject::createElementArray(const string& name, float* vertexData, short* indexData, GLenum primitiveType)
{
	if (this->vao == 0) {
		glGenVertexArrays(1, &this->vao);
	}
	glBindVertexArray(this->vao);
	OGLArrayBuffer* buffer = new OGLArrayBuffer();
	buffer->set(name, vertexData, primitiveType);
	OGLElementArrayBuffer* elementArray = new OGLElementArrayBuffer();
	elementArray->setArrayBuffer(buffer);
	elementArray->set(name, indexData);
	elementArray->generateBuffer();
	this->buffers[name] = elementArray;
	glBindVertexArray(0);
	return (OGLElementArrayBuffer*)this->buffers[name];
}

void OGLObject::createElementArrayPC(const string& name, float* vertexData, short* indexData, GLenum primitiveType)
{
	OGLElementArrayBuffer* elementArray =
		this->createElementArray("Triangles", vertexData, indexData, primitiveType);
	OGLArrayBuffer* ab = elementArray->getArrayBuffer();
	ab->addComponent("Position", 4);
	ab->addComponent("Color", 4);

	ab->setupComponent("Position", 0, GL_FLOAT, 0, 0, 0);

	GLuint numberOfVertices = ab->getNumberOfVertices();
	GLuint positionCount = ab->getComponent("Position")->count;
	GLuint startIdx = numberOfVertices * positionCount;
	GLuint bytesToFirst = startIdx * sizeof(float);
	ab->setupComponent("Color", 1, GL_FLOAT, 0, bytesToFirst, startIdx);

	this->sendBuffersToGPU();
}

void OGLObject::createElementArrayPCN(const string& name, float* vertexData, short* indexData, GLenum primitiveType)
{
	OGLElementArrayBuffer* elementArray =
		this->createElementArray("Triangles", vertexData, indexData, primitiveType);
	OGLArrayBuffer* ab = elementArray->getArrayBuffer();
	ab->addComponent("Position", 4);
	ab->addComponent("Color", 4);
	ab->addComponent("Normal", 3);

	ab->setupComponent("Position", 0, GL_FLOAT, 0, 0, 0);

	GLuint numberOfVertices = ab->getNumberOfVertices();
	GLuint positionCount = ab->getComponent("Position")->count;
	GLuint startIdx = numberOfVertices * positionCount;
	GLuint bytesToFirst = startIdx * sizeof(float);
	ab->setupComponent("Color", 1, GL_FLOAT, 0, bytesToFirst, startIdx);

	GLuint colorCount = ab->getComponent("Color")->count;
	startIdx = numberOfVertices * (positionCount + colorCount);
	bytesToFirst = startIdx * sizeof(float);
	ab->setupComponent("Normal", 2, GL_FLOAT, 0, bytesToFirst, startIdx);

	this->sendBuffersToGPU();
}

void OGLObject::createElementArrayPCNT(const string& name, float* vertexData, short* indexData, GLenum primitiveType)
{
	OGLElementArrayBuffer* elementArray =
		this->createElementArray("Triangles", vertexData, indexData, primitiveType);
	OGLArrayBuffer* ab = elementArray->getArrayBuffer();
	ab->addComponent("Position", 4);
	ab->addComponent("Color", 4);
	ab->addComponent("Normal", 3);
	ab->addComponent("Texture", 2);

	ab->setupComponent("Position", 0, GL_FLOAT, 0, 0, 0);

	GLuint numberOfVertices = ab->getNumberOfVertices();
	GLuint positionCount = ab->getComponent("Position")->count;
	GLuint startIdx = numberOfVertices * positionCount;
	GLuint bytesToFirst = startIdx * sizeof(float);
	ab->setupComponent("Color", 1, GL_FLOAT, 0, bytesToFirst, startIdx);

	GLuint colorCount = ab->getComponent("Color")->count;
	startIdx = numberOfVertices * (positionCount + colorCount);
	bytesToFirst = startIdx * sizeof(float);
	ab->setupComponent("Normal", 2, GL_FLOAT, 0, bytesToFirst, startIdx);

	GLuint normalCount = ab->getComponent("Normal")->count;
	startIdx = numberOfVertices * (positionCount + colorCount + normalCount);
	bytesToFirst = startIdx * sizeof(float);
	ab->setupComponent("Texture", 3, GL_FLOAT, 0, bytesToFirst, startIdx);

	this->sendBuffersToGPU();
}

OGLBufferedObject* OGLObject::getBufferedObject(const string& name)
{
	if (this->buffers.find(name) != this->buffers.end()) {
		return this->buffers[name];
	}
	return nullptr;
}

void OGLObject::renderBufferedObjects()
{
	glBindVertexArray(this->vao);
	auto iterator = this->buffers.begin();
	while (iterator != this->buffers.end()) {
		iterator->second->render();
		iterator++;
	}
	glBindVertexArray(0);
}

void OGLObject::sendMaterialDataToGPU()
{
	auto ambientLocation =
		glGetUniformLocation(
			this->shaderProgram, "materialAmbientIntensity");
	auto shininessLocation =
		glGetUniformLocation(
			this->shaderProgram, "materialShininess");
	auto specularLocation =
		glGetUniformLocation(
			this->shaderProgram, "materialSpecular");

	glUseProgram(this->shaderProgram);
	glUniform1f(ambientLocation, this->material.ambientIntensity);
	glUniform1f(shininessLocation, this->material.shininess);
	glm::vec4 vector =
		glm::vec4(
			this->material.specular.red,
			this->material.specular.green,
			this->material.specular.blue,
			this->material.specular.alpha);
	glUniform4fv(specularLocation, 1, glm::value_ptr(vector));
	glUseProgram(0);
}

