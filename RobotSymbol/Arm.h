#pragma once
#ifndef ARM
#define ARM

#include "OGL3DCompositeObject.h"

#include <string>
using std::string;

class Cuboid;
class Axis;
class Sword;

class Arm :
	public OGL3DCompositeObject
{
protected:
	enum ElbowState { SWING_UP, SWING_DOWN};
protected:
	Cuboid *shoulder;
	Cuboid *bicep;
	Cuboid *forearm;
	Cuboid *hand;
	Sword *sword;

private:
	Axis *axis;
	float elbowDegrees;
	ElbowState state;

public:
	Arm(const string& name);
	virtual ~Arm();

	void setShaderProgram(GLuint shaderProgram);

	void addSword(Sword* sword);

	void update(float elapsedSeconds);

	void render();

};

#endif

