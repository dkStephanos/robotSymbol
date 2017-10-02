#pragma once
#ifndef ROBOT
#define ROBOT

#include "OGL3DCompositeObject.h"

#include <string>
using std::string;

class TexturedCuboid;
class Axis;

class Soldier :
	public OGL3DCompositeObject
{
protected:
	TexturedCuboid *torso, *head, *shoulderL, *shoulderR, *armL, *armR, *legL, *legR, *footL, *footR, *gun, *laser;
	float degrees = 0.0f, armdegrees = 0.0f, shotDistance = 0.0f;
	float deltaRotate = 0, deltaFinalRotate = 0;
	int timer = 0;
	enum State { FORWARD, BACK };
	State state = FORWARD;
	enum Action { STANDING, MOVING, ROTATING, DYING, ATTACKING };
	enum Rotating { ROTATE_TO_MOVE, ROTATE_FROM_MOVE, ROTATE_TO_ATTACK, ROTATE_FROM_ATTACK };
	Action action = STANDING; //ON THE EDGE
	enum Attacking { AIMING, SHOOTING, HOLSTERING
	};
	Attacking attacking = AIMING;
	Rotating rotating;
	int XPos, ZPos;
	bool Living = true, aboutToDie = false, timeToDie = false;
	int power = 30;
	int health = 120;
	float moveDistance = 0, distanceMoved = 0, rotateDistance = 0, rotateAngle = 0, distanceRotated = 0;
	float posX, posZ;

private:
	Axis *axis;

public:
	Soldier(const string& name);
	virtual ~Soldier();

	void setShaderProgram(GLuint shaderProgram);

	void update(float elapsedSeconds);
	void setTorsoTexture(Texture* t);
	void setHeadTexture(Texture* t);
	void setShoulderTexture(Texture* t);
	void setArmTexture(Texture* t);
	void setLegTexture(Texture* t);
	void setFootTexture(Texture* t);
	void setGunTexture(Texture* t);
	void setLaserTexture(Texture* t);

	void setLiving(bool living) { this->Living = living; }
	void setPosition(int XPos, int ZPos) { this->XPos = XPos; this->ZPos = ZPos; }
	void fixLocation(float x, float z);
	void setPosXZ(float x, float z) { this->posX = x; this->posZ = z; }

	void setStanding() { this->action = STANDING; }
	void setMoving() { this->action = MOVING; }
	void setDying() { this->action = DYING; }
	void setAttacking() { this->action = ATTACKING; }
	void setHealth(int health) { this->health = health; }
	void setRotatingToMove() { this->action = ROTATING, this->rotating = ROTATE_TO_MOVE; }
	void setRotatingToAttack() { this->action = ROTATING, this->rotating = ROTATE_TO_ATTACK; }
	void setMoveDistance(float distance) { this->moveDistance = distance; this->distanceMoved = 0.0f; }
	void setRotateAngle(float rotateAngle) { this->rotateAngle = rotateAngle; this->distanceRotated = 0;}

	bool getLiving() { return this->Living; }
	string getState();
	int getXPos() {return this->XPos; }
	int getZPos() { return this->ZPos; }
	int getHealth() { return this->health; }
	int getPower() { return this->power; }

	void render();

};

#endif

