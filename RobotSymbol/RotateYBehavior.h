#pragma once
#ifndef ROTATE_Y_BEHAVIOR
#define ROTATE_Y_BEHAVIOR

#include "IBehavior.h"

class RotateYBehavior :
	public IBehavior
{
protected:
	float speed;

public:
	RotateYBehavior(GameObject *object, float speed);
	virtual ~RotateYBehavior();

	void update(float elapsedSeconds);
};

#endif

