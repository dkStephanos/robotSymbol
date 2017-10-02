#include "OGLFirstPersonCamera.h"
#include "MathUtil.h"

OGLFirstPersonCamera::OGLFirstPersonCamera()
{
	this->leftRight = JUST_LOOKING;
	this->upDown = JUST_LOOKING;
	this->moveState = NOT_MOVING;
	// Should be looking down the z-axis
	this->thetaDegrees = 0.0f;
	this->phiDegrees = 90.0f;
	this->rho = 1.0f;
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
}

OGLFirstPersonCamera::~OGLFirstPersonCamera()
{
}

void OGLFirstPersonCamera::update(float elapsedSeconds)
{
	float lookDelta = 0.0f;
	float moveDelta = 0.0;
	float lookSpeed = 90.0f;
	float moveSpeed = 10.0f;

	glm::vec3 forwardDir = this->lookDir;
	forwardDir.y = 0;
	glm::vec3 rightDir = this->rightDir;
	rightDir.y = 0;

	switch (this->moveState) {
	case NOT_MOVING:
		moveDelta = 0.0;
		break;
	case MOVING_FORWARD:
		this->position += (forwardDir * (moveSpeed * elapsedSeconds));
		break;
	case MOVING_BACKWARD:
		this->position += (-forwardDir * (moveSpeed * elapsedSeconds));
		break;
	case MOVING_RIGHT:
		this->position += (rightDir * (moveSpeed * elapsedSeconds));
		break;
	case MOVING_LEFT:
		this->position += (-rightDir * (moveSpeed * elapsedSeconds));
		break;
	}

	switch (this->leftRight) {
	case JUST_LOOKING:
		lookDelta = 0.0f;
		break;
	case LOOKING_LEFT:
		lookDelta = (lookSpeed * elapsedSeconds);
		this->thetaDegrees += lookDelta;
		if (this->thetaDegrees > 360.0f) {
			this->thetaDegrees = this->thetaDegrees - 360.0f;
		}

		break;
	case LOOKING_RIGHT:
		lookDelta = -(lookSpeed * elapsedSeconds);
		this->thetaDegrees += lookDelta;
		if (this->thetaDegrees < 0.0f) {
			this->thetaDegrees = 360.0f + this->thetaDegrees;
		}
		break;
	}

	switch (this->upDown) {
	case JUST_LOOKING:
		lookDelta = 0.0f;
		break;
	case LOOKING_UP:
		lookDelta = -(lookSpeed * elapsedSeconds);
		this->phiDegrees += lookDelta;
		if (this->phiDegrees < 0.0f) {
			this->phiDegrees = 360.0f + this->phiDegrees;
		}
		break;
	case LOOKING_DOWN:
		lookDelta = (lookSpeed * elapsedSeconds);
		this->phiDegrees += lookDelta;
		if (this->phiDegrees > 360.0f) {
			this->phiDegrees = this->phiDegrees - 360.0f;
		}
		break;
	}

	updateOrientation();
}

void OGLFirstPersonCamera::updateOrientation()
{
	this->cameraSpherical.rho = this->rho;
	this->cameraSpherical.phi = DegToRad(this->phiDegrees);
	this->cameraSpherical.theta = DegToRad(this->thetaDegrees);

	float fSinTheta = sinf(this->cameraSpherical.theta);
	float fCosTheta = cosf(this->cameraSpherical.theta);
	float fCosPhi = cosf(this->cameraSpherical.phi);
	float fSinPhi = sinf(this->cameraSpherical.phi);

	glm::vec3 dirToTarget(fSinTheta * fSinPhi, -fCosPhi, fCosTheta * fSinPhi);
	this->target = this->position + (dirToTarget * this->cameraSpherical.rho);

	this->lookDir = glm::normalize(this->position - this->target);

	glm::vec3 upDir = glm::normalize(this->up);
	this->rightDir = glm::normalize(glm::cross(this->lookDir, upDir));
	glm::vec3 perpUpDir = glm::cross(this->rightDir, this->lookDir);

	glm::mat4 rotMat(1.0f);
	rotMat[0] = glm::vec4(this->rightDir, 0.0f);
	rotMat[1] = glm::vec4(perpUpDir, 0.0f);
	rotMat[2] = glm::vec4(-this->lookDir, 0.0f); // Negate the look direction to point at the look point

	// Since we are building a world-to-camera matrix, we invert the matrix.
	// Transposing a rotation matrix inverts it!
	rotMat = glm::transpose(rotMat);

	// Translate the camera to the origin
	glm::mat4 transMat(1.0f);
	transMat[3] = glm::vec4(-position, 1.0f);

	// Translate and then rotate
	this->orientation = rotMat * transMat;
}
