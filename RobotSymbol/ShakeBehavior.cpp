#include "ShakeBehavior.h"
#include "OGL3DObject.h"

ShakeBehavior::ShakeBehavior(GameObject* object) : IBehavior(object)
{
}

ShakeBehavior::~ShakeBehavior()
{
}

void ShakeBehavior::update(float elapsedSeconds)
{
	OGL3DObject *theObject = (OGL3DObject*)this->object;
	theObject->referenceFrame.translate(0, num, 0);
	num = -num;
}
