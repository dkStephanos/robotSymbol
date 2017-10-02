#include "RotateYBehavior.h"
#include "OGL3DObject.h"

RotateYBehavior::RotateYBehavior(GameObject* object, float speed) : IBehavior(object)
{
	this->speed = speed;
}

RotateYBehavior::~RotateYBehavior()
{
}

void RotateYBehavior::update(float elapsedSeconds)
{
	OGL3DObject *theObject = (OGL3DObject*)this->object;
	theObject->referenceFrame.rotateY(this->speed * elapsedSeconds);
}
