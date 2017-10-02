#pragma once
#ifndef OGL_ELEMENT_ARRAY_BUFFER
#define OGL_ELEMENT_ARRAY_BUFFER

#include "OGLBufferedObject.h"

class OGLArrayBuffer;

class OGLElementArrayBuffer : public OGLBufferedObject
{
protected:
	OGLArrayBuffer* arrayBuffer;

public:
	OGLElementArrayBuffer();

	virtual ~OGLElementArrayBuffer();

	OGLArrayBuffer* getArrayBuffer() { return this->arrayBuffer; }

	void setArrayBuffer(OGLArrayBuffer* buffer) { this->arrayBuffer = buffer; }

	void setTexture(Texture* texture);

	void set(const string& name, short* buffer);

	void setShaderHandle(GLuint shaderHandle);

	void render();

	void drawObject();

	void generateBuffer();

	void sendBufferToGPU();
};

#endif

