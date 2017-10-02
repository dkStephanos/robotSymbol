#pragma once
#ifndef SPHERE
#define SPHERE

#include "OGL3DObject.h"

class Sphere :	public OGL3DObject
{
protected:
	float radius;
	int slices, stacks;
	RGBA surfaceColor;

public:
	Sphere(const string& name, float radius = 1, int slices = 10, int stacks = 10, RGBA surfaceColor = { 1, 1, 1, 1 });
	virtual ~Sphere();

	float getRadius() { return this->radius; }

protected:
	void generate();
};

#endif

