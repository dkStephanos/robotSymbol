#pragma once
#ifndef TEXTURED_CUBOID
#define TEXTURED_CUBOID

#include "BaseCuboid.h"

class TexturedCuboid :	public BaseCuboid
{
public:
	TexturedCuboid(const string& name, float width = 1, float depth = 1, float height = 1, RGBA faceColor = { 1, 1, 1, 1 });

	virtual ~TexturedCuboid();

protected:
	virtual void generate();
};

#endif

