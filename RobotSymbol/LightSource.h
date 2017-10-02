#pragma once
#ifndef LOCAL_LIGHT_SOURCE
#define LOCAL_LIGHT_SOURCE

#include "BaseObject.h"
#include "MathUtil.h"
#include <glm/glm.hpp>

class LightSource : public BaseObject
{
protected:
	glm::vec3 position;
	float intensity;

public:
	LightSource();
	virtual ~LightSource();

	void setPosition(float x, float y, float z) { this->position = glm::vec3(x, y, z); }
	
	glm::vec3 getPosition() { return this->position; }

	void setIntensity(float intensity) { this->intensity = intensity; }

	float getIntensity() { return this->intensity; }

	virtual void update(float elapsedSeconds) {}
};

#endif

