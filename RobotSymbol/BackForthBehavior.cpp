#include "BackForthBehavior.h"
#include "OGLObject.h"

BackForthBehavior::BackForthBehavior(GameObject* object, float speed, float maxDistance) : IBehavior(object)
{
	this->state = MOVING_FORWARD;
	this->maxDistance = maxDistance;
	this->distanceMoved = 0;
	this->speed = speed;
	glm::vec3 dirVector =
		glm::vec3(this->object->referenceFrame.orientation[2]);
	this->object->setVelocity(dirVector * this->speed);
}

BackForthBehavior::~BackForthBehavior()
{
}

void BackForthBehavior::update(float elapsedSeconds)
{
	glm::vec3 dirVector;
	OGLObject* obj = (OGLObject*)this->object;
	float delta = glm::length(obj->getVelocity() * elapsedSeconds);
	this->distanceMoved += delta;
	switch (this->state) {
	case MOVING_BACKWARD:
		if (this->distanceMoved >= this->maxDistance) {
			this->state = MOVING_FORWARD;
			dirVector =	glm::vec3(obj->referenceFrame.orientation[2]);
			this->object->setVelocity(dirVector * this->speed);
			delta = this->distanceMoved - this->maxDistance;
			this->distanceMoved = 0;
		}
		break;
	case MOVING_FORWARD:
		if (this->distanceMoved >= this->maxDistance) {
			this->state = MOVING_BACKWARD;
			dirVector =	glm::vec3(obj->referenceFrame.orientation[2]);
			this->object->setVelocity(-dirVector * this->speed);
			delta = this->distanceMoved - this->maxDistance;
			this->distanceMoved = 0;
		}
		break;
	}
}
