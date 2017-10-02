#include "OGL3DCompositeObject.h"
#include "Axis.h"

OGL3DCompositeObject::OGL3DCompositeObject(const string& name) :
	OGL3DObject(name)
{
	this->axis = nullptr;
}

OGL3DCompositeObject::~OGL3DCompositeObject()
{
	if (this->axis) {
		delete this->axis;
	}
}
