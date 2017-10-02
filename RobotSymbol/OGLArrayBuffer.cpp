#include "OGLArrayBuffer.h"
#include "Texture.h"

OGLArrayBuffer::OGLArrayBuffer()
{
	this->type = GL_ARRAY_BUFFER;
	this->primitiveType = GL_TRIANGLES;
	this->shaderHandle = 0;
	this->usage = GL_STATIC_DRAW;
}

OGLArrayBuffer::~OGLArrayBuffer()
{
	auto it = this->components.begin();
	while (it != this->components.end()) {
		delete it->second;
		it++;
	}
}

void OGLArrayBuffer::set(const string& name, float* buffer, GLenum primitiveType)
{
	this->name = name;
	this->primitiveType = primitiveType;
	this->buffer = &buffer[1];
	this->baseBuffer = buffer;
	this->numberOfElements = (GLuint)buffer[0];
	this->sizeInBytes = this->numberOfElements * sizeof(float);
}

OGLArrayBuffer::Component* OGLArrayBuffer::getComponent(const string& name)
{
	Component* component = nullptr;
	if (this->components.find(name) != this->components.end()) {
		component = this->components[name];
	}
	return component;
}

OGLArrayBuffer::Component* OGLArrayBuffer::addComponent(const string& name, GLuint count)
{
	if (this->components.find(name) == this->components.end()) {
		Component* component = new Component();
		this->components[name] = component;
		this->components[name]->count = count;
	}
	return this->components[name];
}

void OGLArrayBuffer::setupComponent(
	const string& name,
	GLuint layout, GLenum type, GLuint bytesToNext, 
	GLuint bytesToFirst, GLuint startIdx)
{
	Component* component = this->getComponent(name);
	component->layout = layout;
	component->type = type;
	component->bytesToNext = bytesToNext;
	component->bytesToFirst = bytesToFirst;
	component->startIdx = startIdx;
}

GLuint OGLArrayBuffer::getNumberOfVertices()
{
	GLuint totalCount = 0;
	auto it = this->components.begin();
	while (it != this->components.end()) {
		totalCount += it->second->count;
		it++;
	}
	return this->numberOfElements / totalCount;
}

void OGLArrayBuffer::enableComponents()
{
	auto it = this->components.begin();
	while (it != this->components.end()) {
		glEnableVertexAttribArray(it->second->layout);
		glVertexAttribPointer(
			it->second->layout,
			it->second->count,
			it->second->type,
			GL_FALSE,
			it->second->bytesToNext,
			(void*)it->second->bytesToFirst
			);
		it++;
	}
}

void OGLArrayBuffer::disableComponents()
{
	auto it = this->components.begin();
	while (it != this->components.end()) {
		glDisableVertexAttribArray(it->second->layout);
		it++;
	}
}

void OGLArrayBuffer::render()
{
	this->select();
	this->drawObject();
	this->deselect();
}

void OGLArrayBuffer::select()
{
	glUseProgram(this->shaderHandle);
	glBindBuffer(this->type, this->id);
	if (this->texture) {
		this->texture->select();
	}
	this->enableComponents();
}

void OGLArrayBuffer::deselect()
{
	this->disableComponents();
	glUseProgram(0);
}

void OGLArrayBuffer::drawObject()
{
	glDrawArrays(this->primitiveType, 0, this->numberOfElements);
}
