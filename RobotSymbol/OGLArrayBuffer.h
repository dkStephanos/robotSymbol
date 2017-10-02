#pragma once
#ifndef OGL_ARRAY_BUFFER
#define OGL_ARRAY_BUFFER

#include "OGLBufferedObject.h"

#include <unordered_map>
using std::unordered_map;
#include <string>
using std::string;

class OGLArrayBuffer :	public OGLBufferedObject
{
public:
	struct Component {
		GLuint layout;
		GLuint count;
		GLenum type;
		GLuint bytesToNext;
		GLuint bytesToFirst;
		unsigned int startIdx;
	};

protected:
	GLenum primitiveType;

	unordered_map<string, Component*> components;

public:
	OGLArrayBuffer();

	virtual ~OGLArrayBuffer();

	void enableComponents();

	void disableComponents();

	virtual void render();

	void select();

	void deselect();

	GLuint getNumberOfVertices();

	GLenum getPrimitiveType() const { return this->primitiveType; }

	Component* getComponent(const string& name);

	virtual void set(const string& name, float* buffer, GLenum primitiveType);

	virtual OGLArrayBuffer::Component* addComponent(const string& name, GLuint count = 4);

	virtual void setupComponent(
		const string& name, 
		GLuint layout=0, GLenum type=GL_FLOAT, 
		GLuint bytesToNext=0, GLuint bytesToFirst=0, GLuint startIdx = 0);

protected:
	virtual void drawObject();
};

#endif

