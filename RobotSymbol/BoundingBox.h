#pragma once
#ifndef BOUNDING_BOX
#define BOUNDING_BOX

#include "OGLReferenceFrame.h"

#include <glm/glm.hpp>

// Separating Axis Theorem
// -----------------------
// Ref: Huynh, H. (2009).  Separting Axis Theorem for Oriented Bounding Boxes
// In 2D space, the Separating Axis Theorem states two convex polygons do not 
// intersect if and only if there exists a line such that the projections of 
// the two polygons onto the line do not intersect.  The line is known as a 
// separating axis.

class BoundingBox 
{
public:
	// true -> being used to check for overlap
	bool use;

	OGLReferenceFrame referenceFrame;

private:
	float width, depth, height;
	glm::vec3 A, B, C, D;
	glm::vec3 E, F, G, H;

public:
	BoundingBox();

	~BoundingBox(void);

	void applyTransform();

	void set(float width, float depth, float height);

	inline float getWidth() const { return this->width; }
	inline float getHeight() const { return this->height; }
	inline float getDepth() const { return this->depth; }

	bool overlapsWith(const BoundingBox& other);

private:
	// Returns the scalar value of 'point' projected into 'pAxis'
	float getProjection(const glm::vec3& point, const glm::vec3& pAxis) const;

	glm::vec3 getCenter() const;

	glm::vec3 getFrontNormal() const;
	glm::vec3 getUpNormal() const;
	glm::vec3 getRightNormal() const;

	float getMinProjection(const glm::vec3& pAxis) const;
	float getMaxProjection(const glm::vec3& pAxis) const;
};

#endif

