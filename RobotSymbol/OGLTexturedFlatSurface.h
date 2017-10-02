#pragma once
#ifndef OGL_FLAT_SURFACE
#define OGL_FLAT_SURFACE

#include "OGL3DObject.h"

#include <string>
using std::string;

class OGLTexturedFlatSurface :
	public OGL3DObject
{
protected:
	float width, depth;
	unsigned int numberOfWidthSegments, numberOfDepthSegments;
	RGBA surfaceColor;

public:
	OGLTexturedFlatSurface(
		const string& name, 
		float width=5.0f, float depth=5.0f, 
		unsigned int numberOfWidthSegments=5, unsigned int numberOfDepthSegments=5, 
		RGBA surfaceColor = { 1, 1, 1, 1 });

	virtual ~OGLTexturedFlatSurface();

	float getWidth() { return this->width; }

	float getDepth() { return this->depth; }

protected:
	void generate();
};

#endif

