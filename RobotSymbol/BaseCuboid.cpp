#include "BaseCuboid.h"

BaseCuboid::BaseCuboid(const string& name, float width, float depth, float height, RGBA faceColor) :
	OGL3DObject(name)
{
	this->width = width;
	this->depth = depth;
	this->height = height;
	this->faceColor = faceColor;
}

BaseCuboid::~BaseCuboid()
{
}
