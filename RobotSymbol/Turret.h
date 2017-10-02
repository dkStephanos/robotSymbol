#pragma once
#ifndef TURRET
#define TURRET

#include "OGL3DCompositeObject.h"

class Cuboid;

class Turret :
	public OGL3DCompositeObject
{
protected:
	Cuboid *base;
	Cuboid *torso;
	Cuboid *turret;

	float angleY, angleZ, speedZ;

public:
	Turret(const string& name);
	virtual ~Turret();

	void setShaderProgram(GLuint shaderProgram);

	void update(float elapsedSeconds);

	void render();
};

#endif

