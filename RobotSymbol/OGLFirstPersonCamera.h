#pragma once
#ifndef OGL_FIRST_PERSON_CAMERA
#define OGL_FIRST_PERSON_CAMERA

#include "Camera.h"

#include <gl\glew.h>
#include <glm\glm.hpp>

class OGLFirstPersonCamera : public Camera
{
public:
	enum MoveState {NOT_MOVING, MOVING_LEFT, MOVING_RIGHT, MOVING_FORWARD, MOVING_BACKWARD};
	enum LookState {JUST_LOOKING, LOOKING_LEFT, LOOKING_RIGHT, LOOKING_UP, LOOKING_DOWN };

protected:
	struct SphericalCoordinate {
		float rho, phi, theta;
	}cameraSpherical;

	float rho, phiDegrees, thetaDegrees;

	glm::vec3 lookDir;

	glm::vec3 rightDir;

	glm::vec3 position;

	MoveState moveState;
	
	LookState leftRight;

	LookState upDown;

public:
	OGLFirstPersonCamera();

	virtual ~OGLFirstPersonCamera();

	void setPosition(float x, float y, float z) {
		this->position = glm::vec3(x, y, z);
		updateOrientation();
	};

	void update(float elapsedSeconds);

	void setMovingLeft() { this->moveState = MOVING_LEFT; }

	void setMovingRight() { this->moveState = MOVING_RIGHT; }

	void setNotMoving() { this->moveState = NOT_MOVING; }

	void setMovingForward() { this->moveState = MOVING_FORWARD; }

	void setMovingBackward() { this->moveState = MOVING_BACKWARD; }

	void setLookingLeft() { this->leftRight = LOOKING_LEFT; }

	void setLookingRight() { this->leftRight = LOOKING_RIGHT; }

	void setLookingUp() { this->upDown = LOOKING_UP; }

	void setLookingDown() { this->upDown = LOOKING_DOWN; }

	void setJustLooking() { this->upDown = this->leftRight = JUST_LOOKING; }

	glm::vec3 getPosition() { return this->position; }

protected:
	void updateOrientation();
};

#endif

