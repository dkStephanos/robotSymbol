#include "BoundingSphere.h"

#include <cmath>
using std::sqrt;

BoundingSphere::BoundingSphere(const glm::vec3& center, double radius)
{
	this->center = center;
	this->radius = radius;
}

BoundingSphere::BoundingSphere(const BoundingSphere &one,
	const BoundingSphere &two)
{
	glm::vec3 centerOffset = two.center - one.center;
	// Use the square magntude
	double distance = (centerOffset.x * centerOffset.x)
		+ (centerOffset.y * centerOffset.y)
		+ (centerOffset.z * centerOffset.z);
	double radiusDiff = two.radius - one.radius;

	// Check if the larger sphere encloses the small one
	if (radiusDiff * radiusDiff >= distance){
		if (one.radius > two.radius){
			this->center = one.center;
			this->radius = one.radius;
		}
		else{
			this->center = two.center;
			this->radius = two.radius;
		}
	}
	// Otherwise we need to work with partially 
	// overlapping spheres
	else{
		distance = sqrt(distance);
		this->radius = (distance + one.radius + two.radius) * ((double)0.5);
		// The new center is based on one's center, moved towards
		// two's center by an ammount proportional to the spheres'
		// radii.
		this->center = one.center;
		if (distance > 0){
			this->center += centerOffset 
				* (float)((this->radius - one.radius) / distance);
		}
	}
}

bool BoundingSphere::overlapsWith(const BoundingSphere *other) const
{
	glm::vec3 distance = this->center - other->center;
	double distanceSquared = (distance.x * distance.x)
		+ (distance.y * distance.y)
		+ (distance.z * distance.z);
	return distanceSquared 
		< (this->radius + other->radius) * (this->radius + other->radius);
}

double BoundingSphere::getGrowth(const BoundingSphere &other) const
{
	BoundingSphere newSphere(*this, other);
	// We return a value proportional to the change in surface
	// area of the sphere.
	return (newSphere.radius * newSphere.radius) 
		- (this->radius * this->radius);
}

