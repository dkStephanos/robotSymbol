#pragma once
#ifndef MY_OBJECT
#define MY_OBJECT

#include "OGL3DObject.h"
#include "MathUtil.h"

class Diamond : public OGL3DObject
{
public:
	Diamond(const string& name, float scaler);

	virtual ~Diamond();

protected:
	void generate();

	float scaler;

	void update() {};

	//void render() { this->referenceFrame.rotateY(1.0f); }
};

#endif

