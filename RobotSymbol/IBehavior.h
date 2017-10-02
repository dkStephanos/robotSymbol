#pragma once
#ifndef IBEHAVIOR
#define IBEHAVIOR

class GameObject;

class IBehavior
{
protected:
	GameObject *object;
public:
	IBehavior(GameObject *object);
	virtual ~IBehavior();

	virtual void update(float elapsedSeconds) = 0;
};

#endif

