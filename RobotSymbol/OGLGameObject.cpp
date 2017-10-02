#include "OGLGameObject.h"

OGLGameObject::OGLGameObject(const string& name) : GameObject(name)
{
	this->lineBox = nullptr;
}

OGLGameObject::~OGLGameObject()
{
}
