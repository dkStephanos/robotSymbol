#pragma once
#ifndef OGL_SPHERICAL_CAMERA
#define OGL_SPHERICAL_CAMERA

#include "Camera.h"
#include "OGLReferenceFrame.h"

#include <gl\glew.h>
#include <glm\glm.hpp>

/**
 Models a Spherical camera, where the camera is circling about a target point always looking
 at that point.
**/
class OGLSphericalCamera :	public Camera
{
public:
	enum State {NOT_MOVING, MOVING_LEFT, MOVING_RIGHT, MOVING_UP, MOVING_DOWN};

	// See http://mathworld.wolfram.com/SphericalCoordinates.html
	struct SphericalCoordinate {
		float rho, phi, theta;
	}cameraSpherical;

protected:
	float rho, phiDegrees, thetaDegrees;

	State leftRight;

	State upDown;

public:
	OGLSphericalCamera();

	virtual ~OGLSphericalCamera();

	void update(float elapsedSeconds);

	void setPosition(float distanceToTarget, float upDownDegrees, float leftRightDegrees);

	void setMovingLeft() { this->leftRight = MOVING_LEFT; }

	void setMovingRight() { this->leftRight = MOVING_RIGHT; }

	void setNotMovingLeftRight() { this->leftRight = NOT_MOVING; }

	void setMovingUp() { this->upDown = MOVING_UP; }

	void setMovingDown() { this->upDown = MOVING_DOWN; }

	void setNotMovingUpDown() { this->upDown = NOT_MOVING; }

protected:
	void updateOrientation();
};

#endif

