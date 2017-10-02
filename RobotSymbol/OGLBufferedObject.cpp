#include "OGLBufferedObject.h"

OGLBufferedObject::OGLBufferedObject()
{
	this->id = 0;
	this->buffer = nullptr;
	this->baseBuffer = nullptr;
	this->sizeInBytes = 0;
	this->numberOfElements = 0;
	this->texture = nullptr;
	this->usage = GL_STATIC_DRAW;
}

OGLBufferedObject::~OGLBufferedObject()
{
	this->clean();
}

void OGLBufferedObject::clean()
{
	if (this->id != 0) {
		glDeleteBuffers(1, &this->id);
	}
	if (this->buffer) {
		delete this->baseBuffer;
	}
}

void OGLBufferedObject::generateBuffer()
{
	glGenBuffers(1, &this->id);
}

void OGLBufferedObject::sendBufferToGPU()
{
	glBindBuffer(this->type, this->id);
	glBufferData(this->type, this->sizeInBytes, this->buffer, this->usage);
	glBindBuffer(this->type, 0);
}

