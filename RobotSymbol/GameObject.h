#pragma once
#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "BaseObject.h"
#include "RGBA.h"
#include "BoundingBox.h"
#include "OGLReferenceFrame.h"

#include <string>
using std::string;
#include <map>
using std::map;
#include <glm/glm.hpp>

class IBehavior;
class Texture;

struct Material {
	float ambientIntensity; // Light emitted by this object
	RGBA specular; 
	float shininess; 
};

// GameObject -> BaseObject
class GameObject : public BaseObject
{
protected:
	glm::vec3 velocity;

	glm::vec3 acceleration;

	glm::vec3 velocityDelta;

	float damping;

	bool visible;

	IBehavior* behavior;

	Texture* texture;

	bool fixedInPlace;

	map<string, GameObject*> objectsColliding;

public:
	OGLReferenceFrame referenceFrame;

	Material material;

	BoundingBox boundingBox;

	bool showBoundingBox;

	bool ground;

	bool onGround;

public:
	GameObject(string name);

	virtual ~GameObject();

	virtual void setTexture(Texture* texture) { this->texture = texture; }

	void setFixedInPlace(bool fixedInPlace) { this->fixedInPlace = fixedInPlace; }

	bool isFixedInPlace() { return this->fixedInPlace; }

	bool isTextured() { return this->texture != nullptr; }

	void setGround(bool ground) { this->ground = ground; }

	bool isGround() { return this->ground; }

	void setOnGround(bool onGround) { this->onGround = onGround; }

	bool isOnGround() { return this->onGround; }

	virtual void update(float elapsedSeconds);

	virtual void render() = 0;

	void setVisibility(bool visible) { this->visible = visible; }

	bool isVisible() const { return this->visible; }

	void setBehavior(IBehavior* behavior) { this->behavior = behavior; }

	bool checkForCollision(GameObject* otherObject);

	bool isCollidingWith(GameObject* otherObject);

	bool isColliding() { return this->objectsColliding.size() > 0; }

	void clearCollisions() { this->objectsColliding.clear(); }

	void setVelocity(const glm::vec3& velocity) { this->velocity = velocity; }

	void setAcceleration(const glm::vec3& acceleration) { this->acceleration = acceleration; }

	glm::vec3 getVelocity() { return this->velocity; }

	float getSpeed() { return glm::length(this->velocity); }

	virtual glm::vec3 getPosition() { return this->referenceFrame.getPosition(); }

	void stopMoving() {
		this->acceleration = this->velocity = glm::vec3(0, 0, 0);
	}

	virtual void resolveCollisions();
};

#endif

