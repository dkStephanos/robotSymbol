#pragma once
#ifndef PLANE
#define PLANE

#include "OGL3DObject.h"
class Plane : public OGL3DObject
{
protected:
	float height, width;
public:
	Plane(float height, float width);

	virtual ~Plane();

protected:
	void generate();
};

#endif

