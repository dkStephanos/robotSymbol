#include "FallBackBehavior.h"
#include "OGL3DObject.h"

FallBackBehavior::FallBackBehavior(GameObject* object) : IBehavior(object)
{
}

FallBackBehavior::~FallBackBehavior()
{
}

void FallBackBehavior::update(float elapsedSeconds)
{
	OGL3DObject *theObject = (OGL3DObject*)this->object;
	theObject->referenceFrame.translate(0, 2, 0);
	theObject->referenceFrame.rotateX(-20);
}
