#include "Tank.h"
#include "TexturedCuboid.h"


// creates a robot instance. The translations where done via trial and error. 
Tank::Tank(const string& name) :
	OGL3DCompositeObject(name)
{
	this->head = new TexturedCuboid("head", .75, .75, .75, { 0, 0, 1, 1 });
	this->torso = new TexturedCuboid("torso", 1, 1, 1, {0, 0, 1, 1});
	this->arm1 = new TexturedCuboid("arm1", 1, 0.25, 0.25, { 1, 0, 0, 1 });
	this->arm2 = new TexturedCuboid("arm2", 1, 0.25, 0.25, { 1, 0, 0, 1 });
	this->track1 = new TexturedCuboid("track1", 1.5, 0.35, 0.35, { 0, 0, 1, 1 });
	this->track2 = new TexturedCuboid("track2", 1.5, 0.35, 0.35, { 0, 0, 1, 1 });
	this->lazer = new TexturedCuboid("lazer", .5, .1, .1, { .3f, 0, 1, 1 });
	this->lazer1 = new TexturedCuboid("lazer1", .5, .1, .1, { .3f, 0, 1, 1 });
	this->angleY = 0;
	this->delta = 30;
	this->speedZ = 60;
	this->isDead = false;
	this->isStanding = true;
	this->isAttacking = false;
	isRotating = false;
	isMoving = false;
	this->trackMove = .03;
	this->moveTimer = 0;
	this->power = 15;
	this->health = 150;
	this->lazerMove = 0;
	this->lazerStop = 0;
}

void Tank::setTexture(Texture* t)
{
	this->head->setTexture(t);
	this->torso->setTexture(t);
	this->arm1->setTexture(t);
	this->arm2->setTexture(t);
	this->track1->setTexture(t);
	this->track2->setTexture(t);
}

void Tank::setLazerTexture(Texture* t)
{
	this->lazer->setTexture(t);
	this->lazer1->setTexture(t);
}

Tank::~Tank()
{
	delete this->head;
	delete this->torso;
	delete this->arm1;
	delete this->arm2;
	delete this->track1;
	delete this->track2;
	delete this->lazer;
	delete this->lazer1;
}


void Tank::setShaderProgram(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
	this->head->setShaderProgram(this->shaderProgram);
	this->torso->setShaderProgram(this->shaderProgram);
	this->arm1->setShaderProgram(this->shaderProgram);
	this->arm2->setShaderProgram(this->shaderProgram);
	this->track1->setShaderProgram(this->shaderProgram);
	this->track2->setShaderProgram(this->shaderProgram);
	this->lazer->setShaderProgram(this->shaderProgram);
	this->lazer1->setShaderProgram(this->shaderProgram);
}

void Tank::update(float elapsedSeconds)
{
	if (this->isDead == true)
	{
		OGL3DCompositeObject::update(elapsedSeconds);
		this->angleY += (360 * elapsedSeconds);
		if (this->angleY > 360) this->angleY -= 360;
		this->angleZ += (this->speedZ * elapsedSeconds);
		if (this->angleZ > 45) {
			this->speedZ = -60;
		}
		else if (angleZ < -45) {
			this->speedZ = 60;
		}
		this->deathTimer += 2;
		if (this->deathTimer > 100)
		{
			this->deathY += (3 * elapsedSeconds);
			this->referenceFrame.translate(0, -deathY, 0);
		}
	}
	if (this->isMoving == true)
	{
		this->distanceMoved += (moveDistance * elapsedSeconds);
		this->referenceFrame.translate(0, 0, moveDistance * elapsedSeconds);
		if (this->distanceMoved > moveDistance) {
			this->isMoving = false;
			this->isRotating = true;
		}

		this->moveTimer += this->trackMove;
		movingTimer += elapsedSeconds;
		if (this->moveTimer >= .2 || this->moveTimer <= -.2)
		{
			this->trackMove = -this->trackMove;
		}
		if (movingTimer >= 1)
		{
			isMoving = false;
			moveTimer = 0;
			movingTimer = 0;
		}

	}
	if (this->isAttacking == true)
	{
		if (this->lazerMove >= 1.5)
		{
			this->lazerMove = 0;
			this->lazerStop = 0;
			this->isAttacking = false;
			this->isRotating = true;
		}
		else
		{
			this->lazerMove += (3 * elapsedSeconds);
			this->lazerStop += (1.5 * elapsedSeconds);
		}
		if (this->lazerStop >= .5)
		{
			this->lazerStop = 0;
		}
		else
		{
		}
	}

	if (this->isRotating == true)
	{
		switch (rotating)
		{
		case ROTATE_TO_MOVE:
			if (rotateAngle == 0.0f)
			{
				this->isMoving = true;
				this->isRotating = false;
				this->rotating = ROTATE_FROM_MOVE;
				break;
			}
			this->deltaRotate = rotateAngle * elapsedSeconds;
			this->distanceRotated += std::abs(deltaRotate);
			if (distanceRotated <= std::abs(rotateAngle))
			{
				this->referenceFrame.rotateY(-deltaRotate);
			}
			else if (deltaRotate > 0)
			{
				this->deltaFinalRotate = deltaRotate - (distanceRotated - std::abs(rotateAngle));
				this->referenceFrame.rotateY(-deltaFinalRotate);
				this->distanceRotated = std::abs(rotateAngle);
				this->rotating = ROTATE_FROM_MOVE;
				this->isMoving = true;
				this->isRotating = false;
			}
			else
			{
				this->deltaFinalRotate = deltaRotate + (distanceRotated - std::abs(rotateAngle));
				this->referenceFrame.rotateY(-deltaFinalRotate);
				this->distanceRotated = std::abs(rotateAngle);
				this->rotating = ROTATE_FROM_MOVE;
				this->isMoving = true;
				this->isRotating = false;
			}
			break;

		case ROTATE_FROM_MOVE:
			if (rotateAngle == 0.0f)
			{
				this->isStanding = true;
				this->isRotating = false;
				break;
			}
			this->deltaRotate = rotateAngle * elapsedSeconds;
			this->distanceRotated -= std::abs(deltaRotate);
			if (distanceRotated >= 0)
			{
				this->referenceFrame.rotateY(deltaRotate);
			}
			else if (deltaRotate > 0)
			{
				this->deltaFinalRotate = deltaRotate - std::abs(distanceRotated);
				this->referenceFrame.rotateY(deltaFinalRotate);
				fixLocation(posX, posZ);
				this->rotating = ROTATE_TO_MOVE;
				this->isStanding = true;
				this->isRotating = false;
			}
			else
			{
				this->deltaFinalRotate = deltaRotate + std::abs(distanceRotated);
				this->referenceFrame.rotateY(deltaFinalRotate);
				fixLocation(posX, posZ);
				this->rotating = ROTATE_TO_MOVE;
				this->isStanding = true;
				this->isRotating = false;
			}
			break;

		case ROTATE_TO_ATTACK:
			if (rotateAngle == 0.0f)
			{
				this->isAttacking = true;
				this->rotating = ROTATE_FROM_ATTACK;
				break;
			}
			this->deltaRotate = rotateAngle * elapsedSeconds;
			this->distanceRotated += std::abs(deltaRotate);
			if (distanceRotated <= std::abs(rotateAngle))
			{
				this->referenceFrame.rotateY(-deltaRotate);
			}
			else if (deltaRotate > 0)
			{
				this->deltaFinalRotate = deltaRotate - (distanceRotated - std::abs(rotateAngle));
				this->referenceFrame.rotateY(-deltaFinalRotate);
				this->distanceRotated = std::abs(rotateAngle);
				this->rotating = ROTATE_FROM_ATTACK;
				this->isAttacking = true;
				this->isRotating = false;
			}
			else
			{
				this->deltaFinalRotate = deltaRotate + (distanceRotated - std::abs(rotateAngle));
				this->referenceFrame.rotateY(-deltaFinalRotate);
				this->distanceRotated = std::abs(rotateAngle);
				this->rotating = ROTATE_FROM_ATTACK;
				this->isAttacking = true;
				this->isRotating = false;
			}
			break;

		case ROTATE_FROM_ATTACK:
			if (rotateAngle == 0.0f)
			{
				this->isStanding = true;
				this->isRotating = false;
				break;
			}
			this->deltaRotate = rotateAngle * elapsedSeconds;
			this->distanceRotated -= std::abs(deltaRotate);
			if (distanceRotated >= 0)
			{
				this->referenceFrame.rotateY(deltaRotate);
			}
			else if (deltaRotate > 0)
			{
				this->deltaFinalRotate = deltaRotate - std::abs(distanceRotated);
				this->referenceFrame.rotateY(deltaFinalRotate);
				this->rotating = ROTATE_TO_ATTACK;
				this->isStanding = true;
				this->isRotating = false;
			}
			else
			{
				this->deltaFinalRotate = deltaRotate + std::abs(distanceRotated);
				this->referenceFrame.rotateY(deltaFinalRotate);
				this->rotating = ROTATE_TO_ATTACK;
				this->isStanding = true;
				this->isRotating = false;
			}
			break;
		}
	}//end of if(isRotating)

}

void Tank::render()
{
	this->head->referenceFrame = this->referenceFrame;
	this->frameStack.setBaseFrame(this->head->referenceFrame);
	this->frameStack.rotateY(angleY);
	this->head->render();
	this->frameStack.push();
	{
		this->frameStack.translate(0, -.9f, 0);
		this->torso->render(this->frameStack.top());
		this->frameStack.push();
		{
			this->frameStack.translate(.63f, 0, .50f);
			this->frameStack.rotateY(90);
			this->frameStack.rotateZ(angleY);
			this->frameStack.translate(lazerStop, 0, 0);
			this->arm1->render(frameStack.top());
			this->frameStack.push();
			{
				this->frameStack.translate(-lazerMove, 0, 0);
				this->lazer->render(frameStack.top());
			}
			this->frameStack.pop();
		}
		this->frameStack.pop();
		this->frameStack.push();
		{
			this->frameStack.translate(-.63f, 0, .50f);
			this->frameStack.rotateY(90);
			this->frameStack.rotateZ(-angleY);
			this->frameStack.translate(lazerStop, 0, 0);
			this->arm2->render(frameStack.top());
			this->frameStack.push();
			{
				this->frameStack.translate(-lazerMove, 0, 0);
				this->lazer->render(frameStack.top());
			}
			this->frameStack.pop();
		}
		this->frameStack.pop();
		this->frameStack.push();
		{
			this->frameStack.translate(.60f, -.70f, 0);
			this->frameStack.rotateY(90);
			this->frameStack.translate(moveTimer, 0, 0);
			this->track2->render(frameStack.top());
		}
		this->frameStack.pop();
		this->frameStack.push();
		{
			this->frameStack.translate(-.60f, -.70f, 0);
			this->frameStack.rotateY(90);
			this->frameStack.translate(moveTimer, 0, 0);
			this->track1->render(frameStack.top());
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();
}

string Tank::getState()
{
	if (this->isStanding)
		return "STANDING";
	else if (this->isDead)
		return "DYING";
	else if (this->isAttacking)
		return "ATTACKING";
	else if (this->isRotating)
		return "ROTATING";
	else if (this->isMoving)
		return "MOVING";
}

void Tank::fixLocation(float x, float z)
{
	this->referenceFrame.setPosition(x, -3.175f, z);
}