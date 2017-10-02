#include "GameObject.h"
#include "IBehavior.h"
#include "OGLReferenceFrame.h"

GameObject::GameObject(string name)
{
	this->name = name;
	this->visible = false;
	this->behavior = nullptr;
	this->material.ambientIntensity = 0.1f; 
	this->material.specular = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->material.shininess = 0.000001f;
	this->texture = nullptr;
	this->showBoundingBox = false;
	this->damping = 0.99f;
	this->fixedInPlace = true;
	this->ground = false;
	this->onGround = false;

}


GameObject::~GameObject()
{
	if (this->behavior) {
		delete this->behavior;
	}
}

bool GameObject::checkForCollision(GameObject* otherObject)
{
	// Don't check the fixed objects
	if (!this->fixedInPlace) {
		// Can't collide with self
		if (this->getName() != otherObject->getName()) {
			if (!this->isCollidingWith(otherObject)) {
				if (this->boundingBox.overlapsWith(otherObject->boundingBox)) {
					this->objectsColliding[otherObject->getName()] = otherObject;
					otherObject->objectsColliding[this->getName()] = this;
					if (otherObject->isGround()) {
						this->onGround = true;
					}
					return true;
				}
			}
		}
	}
	return false;
}

bool GameObject::isCollidingWith(GameObject* otherObject)
{
	return this->objectsColliding.find(otherObject->getName()) != this->objectsColliding.end();
}

void GameObject::update(float elapsedSeconds)
{
	if (this->behavior) {
		this->behavior->update(elapsedSeconds);
	}
	
	if (this->fixedInPlace) return;
	// Linear movement
	this->velocityDelta = this->velocity * elapsedSeconds;
	this->referenceFrame.move(this->velocityDelta);
	this->velocity += this->acceleration * elapsedSeconds;
	this->velocity *= powf(this->damping, elapsedSeconds);
}

void GameObject::resolveCollisions()
{
	if (this->isColliding()) {
		this->referenceFrame.move(-this->velocityDelta);
		this->stopMoving();
	}
	else {
		if (!this->onGround) {
			this->setAcceleration(glm::vec3(0, -32.2f, 0));
		}

	}
}

