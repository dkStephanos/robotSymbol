#pragma once
#include "OGL3DCompositeObject.h"
#include "TexturedCuboid.h"
#include "OGLTexturedFlatSurface.h"
#include "TextFileReader.h"
#include <sstream>

class TexturedCuboid;
class OGLTexturedFlatSurface;

class Flyer :
	public OGL3DCompositeObject
{
protected:
	TexturedCuboid *base, *leftArm, *leftHand, *rightArm, *rightHand, *body, *chest, *laser, *head;
	OGLTexturedFlatSurface *face;

	enum Action { STANDING, MOVING, DYING, ATTACKING, ROTATING };
	enum Attacking { AIMING, SHOOTING, HOLSTERING };
	enum Moving { TAKE_OFF, FLYING, LANDING };
	enum Dying { SPINNING, EXPLODING };
	Action action = STANDING; //HERE, I REALIZE
	Attacking attacking = AIMING;
	Moving moving = TAKE_OFF;
	Dying dying = SPINNING;
	enum Rotating { ROTATE_TO_MOVE, ROTATE_FROM_MOVE, ROTATE_TO_ATTACK, ROTATE_FROM_ATTACK };
	Rotating rotating;

	int XPos, ZPos;
	bool Living = true;
	int power = 45;
	int health = 90;
	float flyRotateAngle = 0, flyTakeOffDistance = 0, flyDistance = 0;
	float attackRotateAngle = 0, attackDistance = 0;
	float dyingRotateAngle = 0, dyingDistance = 0;
	float moveDistance = 0, rotateDistance = 0, rotateAngle = 0, distanceRotated = 0;
	float deltaRotate = 0, deltaFinalRotate = 0;
	float posX, posZ;

	TextFileReader *reader;

public:
	Flyer(const string& name);
	~Flyer();

	void update(float elapsedSeconds);

	void render();

	void setShaderProgram(GLuint shaderProgram);

	void setLiving(bool living) { this->Living = living; }
	void setPosition(int XPos, int ZPos) { this->XPos = XPos; this->ZPos = ZPos; }
	void setMoveDistance(float distance) { this->moveDistance = distance; this->flyDistance = 0; }
	void setRotateAngle(float rotateAngle) { this->rotateAngle = rotateAngle; this->distanceRotated = 0; }

	void setStanding() { this->action = STANDING; }
	void setMoving() { this->action = MOVING; }
	void setRotatingToMove() { this->action = ROTATING, this->rotating = ROTATE_TO_MOVE; }
	void setRotatingToAttack() { this->action = ROTATING, this->rotating = ROTATE_TO_ATTACK; }
	void setDying() { this->action = DYING; }
	void setAttacking() { this->action = ATTACKING; }
	void setHealth(int health) { this->health = health; }
	void fixLocation(float x, float z);
	void setPosXZ(float x, float z) { this->posX = x; this->posZ = z; }

	bool getLiving() { return this->Living; }
	string getState();
	int getXPos() { return this->XPos; }
	int getZPos() { return this->ZPos; }
	int getHealth() { return this->health; }
	int getPower() { return this->power; }

	void setRobotTexture(Texture *t);
	void setFaceTexture(Texture *t) { this->face->setTexture(t); }
	void setLaserTexture(Texture *t);

};