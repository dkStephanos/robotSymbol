#pragma once
#include "OGL3DCompositeObject.h"

#ifndef TANK
#define TANK

class TexturedCuboid;

class Tank :
	public OGL3DCompositeObject
{
protected:
	TexturedCuboid *head;
	TexturedCuboid *torso;
	TexturedCuboid *arm1;
	TexturedCuboid *arm2;
	TexturedCuboid *track1;
	TexturedCuboid *track2;
	TexturedCuboid *lazer;
	TexturedCuboid *lazer1;

	enum Action { STANDING, MOVING, DYING, ROTATING, ATTACKING };
	enum Rotating { ROTATE_TO_MOVE, ROTATE_FROM_MOVE, ROTATE_TO_ATTACK, ROTATE_FROM_ATTACK };
	Action action = STANDING; //in the middle with you?
	Rotating rotating;
	bool isDead;
	bool isMoving;
	bool isAttacking;
	bool isRotating;
	bool isStanding;
	int XPos, ZPos;
	bool Living = true;
	int power;
	int health;
	float moveDistance = 0, distanceMoved = 0, rotateDistance = 0, rotateAngle = 0, distanceRotated = 0;
	float deltaRotate = 0, deltaFinalRotate = 0;
	float posX, posZ;

	float angleY, angleZ = 0, delta, speedZ, deathTimer = 0, deathY = 0, trackMove, moveTimer, lazerMove, lazerStop, movingTimer = 0;

public:
	Tank(const string& name);
	virtual ~Tank();

	void setShaderProgram(GLuint shaderProgram);
	void setTexture(Texture * t);
	void setLazerTexture(Texture * t);
	void update(float elapsedSeconds);

	void setLiving(bool living) { this->Living = living; }
	void setPosition(int XPos, int ZPos) { this->XPos = XPos; this->ZPos = ZPos; }
	void fixLocation(float x, float z);
	void setPosXZ(float x, float z) { this->posX = x; this->posZ = z; }

	void setStanding() { this->action = STANDING; }
	void setMoving() { isMoving = true; }
	void setDying() { isDead = true; }
	void setAttacking() { isAttacking = true; }
	void setHealth(int health) { this->health = health; }
	void setRotatingToMove() { this->isRotating = true, this->rotating = ROTATE_TO_MOVE; }
	void setRotatingToAttack() { this->isRotating = true, this->rotating = ROTATE_TO_ATTACK; }
	void setMoveDistance(float distance) { this->moveDistance = distance; this->distanceMoved = 0.0f; }
	void setRotateAngle(float rotateAngle) { this->rotateAngle = rotateAngle;  this->distanceRotated = 0;
	}

	bool getLiving() { return this->Living; }
	int getXPos() { return this->XPos; }
	int getZPos() { return this->ZPos; }
	int getHealth() { return this->health; }
	int getPower() { return this->power; }
	string getState();

	void render();
};

#endif
