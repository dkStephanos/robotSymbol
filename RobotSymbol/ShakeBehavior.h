#pragma once
#ifndef SHAKE_BEHAVIOR
#define SHAKE_BEHAVIOR

#include "IBehavior.h"

class ShakeBehavior :
	public IBehavior
{
protected:
	float num = .1;

public:
	ShakeBehavior(GameObject *object);
	virtual ~ShakeBehavior();

	void update(float elapsedSeconds);
};

#endif

