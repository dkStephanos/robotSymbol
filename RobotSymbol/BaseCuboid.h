#pragma once
#ifndef BASE_CUBOID
#define BASE_CUBOID

#include "OGL3DObject.h"

class BaseCuboid :	public OGL3DObject
{
protected:
	float width, depth, height;
	RGBA faceColor;

public:
	BaseCuboid(const string& name, float width = 1, float depth = 1, float height = 1, RGBA faceColor = { 1, 1, 1, 1 });
	
	virtual ~BaseCuboid();

	float getWidth() { return this->width; }

	float getHeight() { return this->height; }

	float getDepth() { return this->depth; }

protected:
	virtual void generate() = 0;

};

#endif

