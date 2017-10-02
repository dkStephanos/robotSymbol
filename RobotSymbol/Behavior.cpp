#include "IBehavior.h"
#include "GameObject.h"

IBehavior::IBehavior(GameObject *object)
{
	this->object = object;
}

IBehavior::~IBehavior()
{
}
