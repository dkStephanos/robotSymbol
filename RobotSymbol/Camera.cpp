#include "Camera.h"

Camera::Camera() : GameObject("Camera")
{
}

Camera::~Camera()
{
}

void Camera::setTarget(float x, float y, float z)
{
	this->target = glm::vec3(x, y, z);
	this->updateOrientation();
}

void Camera::setUp(float x, float y, float z)
{
	this->up = glm::vec3(x, y, z);
	this->updateOrientation();
}

void Camera::updateOrientation()
{
}

glm::vec3 Camera::getPosition()
{
	return glm::vec3(this->orientation[3]);
}