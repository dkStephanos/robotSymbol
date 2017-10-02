#pragma once
#ifndef PYRAMID
#define PYRAMID

#include "OGL3DObject.h"
#include "RGBA.h"

class Pyramid :	public OGL3DObject
{
protected:
	float width, depth, height;
	RGBA color;

public:
	Pyramid(const string& name, float width=1, float depth=1, float height=1, RGBA color = { 1, 1, 1, 1 });
	
	virtual ~Pyramid();

	float getWidth() { return this->width; }
	float getDepth() { return this->depth; }
	float getHeight() { return this->height; }

protected:
	void generate();
};

#endif

