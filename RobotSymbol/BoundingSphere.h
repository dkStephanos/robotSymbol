#pragma once
#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

// ----------------------------------------------------------------------------
// Reference:
// Millington, I. (2010). Game Physics Engine Development. 2nd Edition. 
// Morgan Kaufman
// ----------------------------------------------------------------------------

#include "MathUtil.h"
#include <glm\glm.hpp>

class BoundingSphere
{
public:
	glm::vec3 center;

	double radius;

public:
	// Creates a new bounding sphere at the given centre and radius.
	//
	BoundingSphere(const glm::vec3& center=glm::vec3(0, 0, 0), double radius=1.0);

	// Creates a bounding sphere to enclose the two given bounding
	// spheres.
	BoundingSphere(const BoundingSphere& one, const BoundingSphere& two);

	// Checks if the bounding sphere overlaps with the other given
	// bounding sphere.
	bool overlapsWith(const BoundingSphere *other) const;

	// Reports how much this bounding sphere would have to grow
	// by to incorporate the given bounding sphere. Note that this
	// calculation returns a value not in any particular units (i.e.
	// its not a volume growth). In fact the best implementation
	// takes into account the growth in surface area (after the
	// Goldsmith-Salmon algorithm for tree construction).
	double getGrowth(const BoundingSphere &other) const;

	// Returns the volume of this bounding volume. This is used
	// to calculate how to recurse into the bounding volume tree.
	// For a bounding sphere it is a simple calculation.
	double getSize() const
	{
		return ((double)1.333333) * PI_CONST 
			* this->radius * this->radius * this->radius;
	}
};

#endif

