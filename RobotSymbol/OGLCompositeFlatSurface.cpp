#include "OGLCompositeFlatSurface.h"
#include "OGLTexturedFlatSurface.h"

OGLCompositeFlatSurface::OGLCompositeFlatSurface(const string& name) 
	: OGL3DCompositeObject(name)
{
}

OGLCompositeFlatSurface::~OGLCompositeFlatSurface()
{
	for (size_t i = 0; i < this->surfaces.size(); i++) {
		delete this->surfaces[i];
	}
}

void OGLCompositeFlatSurface::setShaderProgram(GLuint shaderProgram)
{
}

void OGLCompositeFlatSurface::update(float elapsedSeconds)
{
	OGL3DCompositeObject::update(elapsedSeconds);
}

void OGLCompositeFlatSurface::render()
{
	this->frameStack.setBaseFrame(this->referenceFrame);
	this->frameStack.push();
	{

	}
	this->frameStack.pop();
}
