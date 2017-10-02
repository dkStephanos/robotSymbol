#pragma once
#ifndef OGL_BUFFERED_OBJECT
#define OGL_BUFFERED_OBJECT

#include "BaseObject.h"

#include <gl\glew.h>

class Texture;

class OGLBufferedObject : public BaseObject
{
protected:
	GLuint id;

	// GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER
	GLenum type; 

	// Must be allocated on the heap, the first element stores
	// the number of elements
	void* buffer;
	void* baseBuffer;

	GLuint sizeInBytes;

	GLuint numberOfElements;

	// GL_STATIC_DRAW or GL_DYNAMIC_DRAW
	GLenum usage;

	Texture* texture;

	GLuint shaderHandle;

public:
	OGLBufferedObject();

	virtual ~OGLBufferedObject();

	void clean();

	GLuint getId() const { return this->id; }

	virtual void setTexture(Texture* texture) { this->texture = texture; }

	void setUsage(GLenum usage) { this->usage = usage; }

	GLuint getNumberOfElements() const { return this->numberOfElements; }

	GLuint getShaderHandle() const { return this->shaderHandle; }

	virtual void setShaderHandle(GLuint shaderHandle) { this->shaderHandle = shaderHandle; }

	virtual void generateBuffer();

	virtual void sendBufferToGPU();

	virtual void render() = 0;
};

#endif

