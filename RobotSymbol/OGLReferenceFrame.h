#pragma once
#ifndef OGL_REFERENCE_FRAME
#define OGL_REFERENCE_FRAME

#include "MathUtil.h"

#include <glm\glm.hpp>
#include <string>
using std::string;
#include <vector>
using std::vector;

class OGLReferenceFrame
{
public:
	glm::mat4 orientation;

	vector<Transform> transforms;

public:
	OGLReferenceFrame();
	virtual ~OGLReferenceFrame();

	// Sets the position 
	void setPosition(const glm::vec3& position);
	void setPosition(float x, float y, float z);

	// Gets the position
	glm::vec3 getPosition() { return glm::vec3(orientation[3]); }

	void rotateZ(float degrees);

	void rotateWorldZ(float degrees);

	void rotateY(float degrees);

	void rotateWorldY(float degrees);

	void rotateX(float degrees);

	void rotateWorldX(float degrees);

	void move(const glm::vec3& direction, float speed);

	void move(const glm::vec3& velocity);

	void moveForward(float speed);

	void moveBackward(float speed);

	void translate(float dx, float dy, float dz);

	void translateWorld(float dx, float dy, float dz);

	void addTransform(const Transform& transform) {
		this->transforms.push_back(transform);
	}

	void applyTransforms();
};

#endif

