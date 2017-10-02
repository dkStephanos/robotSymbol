#pragma once
#ifndef AXIS
#define AXIS

#include <string>
using std::string;

#include "OGL3DObject.h"
class Axis :
	public OGL3DObject
{
protected:
	float length;

public:
	Axis(const string& name, float length=1.0f);

	virtual ~Axis();

protected:
	void generate();
};

#endif

