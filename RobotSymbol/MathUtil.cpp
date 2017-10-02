#include "MathUtil.h"
#include <cmath>

void Vector3f::normalize()
{
	float length = sqrtf(this->v1 * this->v1 + this->v2 * this->v2 + this->v3 * this->v3);
	this->v1 = this->v1 / length;
	this->v2 = this->v2 / length;
	this->v3 = this->v3 / length;
}

Vector3f Vector3f::getNormal()
{
	float length = sqrtf(this->v1 * this->v1 + this->v2 * this->v2 + this->v3 * this->v3);
	return{ this->v1 / length, this->v2 / length, this->v3 / length };
}