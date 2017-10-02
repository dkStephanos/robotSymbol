#pragma once
#ifndef SWORD
#define SWORD

#include <string>
using std::string;

#include "OGL3DCompositeObject.h"

class Cuboid;
class Axis;

class Sword :
	public OGL3DCompositeObject
{
protected:
	Cuboid *grip;
	Cuboid *guard;
	Cuboid *blade;
private:
	Axis *axis;

public:
	Sword(const string& name);
	virtual ~Sword();

	void setShaderProgram(GLuint shaderProgram);

	void update(float elapsedSeconds);

	void render();
};

#endif

