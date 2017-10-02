#pragma once
#ifndef OGL_OBJECT
#define OGL_OBJECT

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <map>
using std::map;
#include <string>
using std::string;

#include "OGLGameObject.h"
#include "OGLReferenceFrame.h"

class OGLBufferedObject;
class OGLElementArrayBuffer;

// OGLObject -> OGLGameObject -> GameObject -> BaseObject
class OGLObject : public OGLGameObject
{
protected:
	map<string, OGLBufferedObject*> buffers;

	GLenum targetBufferObject;
	
public:
	OGLObject(string name);

	virtual ~OGLObject();

	void update(float elapsedSeconds);

	void render() = 0;

	virtual void render(OGLReferenceFrame& frame) = 0;

	void setTexture(Texture* texture);

	void setShaderProgram(GLuint shaderProgram);

	void sendBuffersToGPU();

	OGLBufferedObject* getBufferedObject(const string& name);

	void createArrayBuffer(string name, float* vertexData, GLuint numberOfElements, GLenum primitiveType);

	OGLElementArrayBuffer* createElementArray(const string& name, float* vertexData, short* indexData, GLenum primitiveType = GL_TRIANGLES);

	void createElementArrayPC(const string& name, float* vertexData, short* indexData, GLenum primitiveType = GL_TRIANGLES);
	void createElementArrayPCN(const string& name, float* vertexData, short* indexData, GLenum primitiveType = GL_TRIANGLES);
	void createElementArrayPCNT(const string& name, float* vertexData, short* indexData, GLenum primitiveType = GL_TRIANGLES);

	void sendMaterialDataToGPU();

protected:
	void renderBufferedObjects();
};

#endif

