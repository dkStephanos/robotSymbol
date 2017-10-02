#pragma once
#ifndef FALL_BACK_BEHAVIOR
#define FALL_BACK_BEHAVIOR

#include "IBehavior.h"

class FallBackBehavior :
	public IBehavior
{
protected:
	float num = .1;

public:
	FallBackBehavior(GameObject *object);
	virtual ~FallBackBehavior();

	void update(float elapsedSeconds);
};

#endif

