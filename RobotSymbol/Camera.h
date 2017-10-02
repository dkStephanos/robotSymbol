#pragma once
#ifndef CAMERA
#define CAMERA

#include "GameObject.h"

#include <glm\glm.hpp>

class Camera : public GameObject
{
protected:
	glm::vec3 up;
	glm::vec3 target;

public:
	glm::mat4 orientation;

public:
	Camera();

	virtual ~Camera();

	virtual void setTarget(float x, float y, float z);

	virtual void setUp(float x, float y, float z);
	
	virtual void update(float elapsedSeconds) {}

	glm::vec3 getPosition();

	void render() {}

protected:
	virtual void updateOrientation();
};

#endif

