#include "Soldier.h"
#include "TexturedCuboid.h"
#include "Axis.h"
#include "FallBackBehavior.h"

Soldier::Soldier(const string& name) : OGL3DCompositeObject(name)
{
	this->torso = new TexturedCuboid("torso", 1.0f, 1.0f, 1.5f, { 0.55f, 0.55f, 0.55f });
	this->head = new TexturedCuboid("head", 0.5f, 0.5f, 0.5f, { 0.75f, 0.75f, 0.75f });
	this->shoulderL = new TexturedCuboid("shoulderL", 0.5f, 0.5f, 0.5f, { 0.85f, 0.85f, 0.0f });
	this->shoulderR = new TexturedCuboid("shoulderR", 0.5f, 0.5f, 0.5f, { 0.85f, 0.85f, 0.0f });
	this->armL = new TexturedCuboid("armL", 0.25f, 0.25f, 0.75f, { 0.75f, 0.75f, 0.75f });
	this->armR = new TexturedCuboid("armR", 0.25f, 0.25f, 0.75f, { 0.75f, 0.75f, 0.75f });
	this->legL = new TexturedCuboid("legL", 0.375f, 0.25f, 0.625f, { 0.75f, 0.75f, 0.75f });
	this->legR = new TexturedCuboid("legR", 0.375f, 0.25f, 0.625f, { 0.75f, 0.75f, 0.75f });
	this->footL = new TexturedCuboid("footL", 0.375f, 0.625f, 0.375f, { 0.55f, 0.55f, 0.55f });
	this->footR = new TexturedCuboid("footR", 0.375f, 0.625f, 0.375f, { 0.55f, 0.55f, 0.55f });
	this->gun = new TexturedCuboid("gun", 0.25f, 0.25f, 0.5f, { 0.5f, 0.5f, 0.5f });
	
	this->laser = new TexturedCuboid("laser", 0.2f, 0.2f, 0.3f, { .69f, .73f, .76f, 0.0f });
	this->axis = new Axis("Axes", 2.0f);
}

Soldier::~Soldier()
{
	delete this->torso;
	delete this->head;
	delete this->shoulderL;
	delete this->armL;
	delete this->legL;
	delete this->footL;
	delete this->shoulderR;
	delete this->armR;
	delete this->legR;
	delete this->footR;
	delete this->gun;
	delete this->axis;
	delete this->laser;
}

void Soldier::setShoulderTexture(Texture* t)
{
	this->shoulderL->setTexture(t);
	this->shoulderR->setTexture(t);
}

void Soldier::setTorsoTexture(Texture* t)
{
	this->torso->setTexture(t);
}

void Soldier::setHeadTexture(Texture* t)
{
	this->head->setTexture(t);
}

void Soldier::setArmTexture(Texture* t)
{
	this->armL->setTexture(t);
	this->armR->setTexture(t);
}

void Soldier::setLegTexture(Texture* t)
{
	this->legL->setTexture(t);
	this->legR->setTexture(t);
}

void Soldier::setFootTexture(Texture* t)
{
	this->footL->setTexture(t);
	this->footR->setTexture(t);
}

void Soldier::setGunTexture(Texture* t)
{
	this->gun->setTexture(t);
}

void Soldier::setLaserTexture(Texture* t)
{
	this->laser->setTexture(t);
}

void Soldier::setShaderProgram(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
	this->torso->setShaderProgram(this->shaderProgram);
	this->head->setShaderProgram(this->shaderProgram);
	this->shoulderL->setShaderProgram(this->shaderProgram);
	this->armL->setShaderProgram(this->shaderProgram);
	this->legL->setShaderProgram(this->shaderProgram);
	this->footL->setShaderProgram(this->shaderProgram);
	this->shoulderR->setShaderProgram(this->shaderProgram);
	this->armR->setShaderProgram(this->shaderProgram);
	this->legR->setShaderProgram(this->shaderProgram);
	this->footR->setShaderProgram(this->shaderProgram);
	this->gun->setShaderProgram(this->shaderProgram);
	this->axis->setShaderProgram(this->shaderProgram);
	this->laser->setShaderProgram(this->shaderProgram);
}

string Soldier::getState()
{
	switch (this->action)
	{
	case (STANDING) :
		return "STANDING";
		break;
	case (MOVING) :
		return "MOVING";
		break;
	case (DYING) :
		return "DYING";
		break;
	case (ATTACKING) :
		return "ATTACKING";
		break;
	case (ROTATING) :
		return "ROTATING";
		break;
	}
}

void Soldier::update(float elapsedSeconds)
{
	OGL3DCompositeObject::update(elapsedSeconds);

	switch (this->action)
	{
	case STANDING:
		this->degrees = 0;
		break;
	case MOVING:
		switch (this->state)
		{
		case FORWARD:
			this->distanceMoved += (moveDistance * elapsedSeconds);
			this->referenceFrame.translate(0, 0, moveDistance * elapsedSeconds);
			if (this->distanceMoved > moveDistance) {
				this->action = ROTATING;
			}
			this->degrees += (130.0f * elapsedSeconds);
			if (this->degrees > 45.0f)
				this->state = BACK;
			if (this->degrees > -1 && this->degrees < 0)
			{
				this->action = STANDING;
			}
			break;
		case BACK:
			this->distanceMoved += (moveDistance * elapsedSeconds);
			this->referenceFrame.translate(0, 0, moveDistance * elapsedSeconds);
			if (this->distanceMoved > moveDistance) {
				this->action = ROTATING;
			}
			this->degrees -= (130.0f * elapsedSeconds);
			if (this->degrees < -45.0f)
			{
				this->state = FORWARD;
			}
			break;
		}
		break;

	case ATTACKING:
		switch (attacking)
		{
		case AIMING:
			if (this->armdegrees > -75.0f)
			{
				this->armdegrees -= (75.0f * elapsedSeconds);
			}
			if (this->armdegrees <= -75.0f) {
				this->attacking = SHOOTING;
			}
			break;

		case SHOOTING:
			this->shotDistance += (8.0f * elapsedSeconds);
			if (this->shotDistance > 2.0f) {
				shotDistance = 0.0f;
				this->attacking = HOLSTERING;
			}
			break;

		case HOLSTERING:
			if (this->armdegrees < 0.0f)
			{
				this->armdegrees += (75.0f * elapsedSeconds);
			}
			if (this->armdegrees >= 0.0f) {
				armdegrees = 0.0f;
				this->attacking = AIMING;
				this->action = ROTATING;
			}
			break;
		}
		break;

	case DYING:
		timer++;
		if (timer > 60 && timer < 75)
		{
			timeToDie = true;
			this->setBehavior(new FallBackBehavior(this));
		}
		else if (timer > 75)
			this->setVisibility(false);
		break;

	case ROTATING:
		switch (rotating)
		{
		case ROTATE_TO_MOVE:
			if (rotateAngle == 0.0f)
			{
				this->action = MOVING;
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
				this->action = MOVING;
			}
			else
			{
				this->deltaFinalRotate = deltaRotate + (distanceRotated - std::abs(rotateAngle));
				this->referenceFrame.rotateY(-deltaFinalRotate);
				this->distanceRotated = std::abs(rotateAngle);
				this->rotating = ROTATE_FROM_MOVE;
				this->action = MOVING;
			}
			break;

		case ROTATE_FROM_MOVE:
			if (rotateAngle == 0.0f)
			{
				this->action = STANDING;
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
				this->action = STANDING;
			}
			else
			{
				this->deltaFinalRotate = deltaRotate + std::abs(distanceRotated);
				this->referenceFrame.rotateY(deltaFinalRotate);
				fixLocation(posX, posZ);
				this->rotating = ROTATE_TO_MOVE;
				this->action = STANDING;
			}
			break;

		case ROTATE_TO_ATTACK:
			if (rotateAngle == 0.0f)
			{
				this->action = ATTACKING;
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
				this->action = ATTACKING;
			}
			else
			{
				this->deltaFinalRotate = deltaRotate + (distanceRotated - std::abs(rotateAngle));
				this->referenceFrame.rotateY(-deltaFinalRotate);
				this->distanceRotated = std::abs(rotateAngle);
				this->rotating = ROTATE_FROM_ATTACK;
				this->action = ATTACKING;
			}
			break;

		case ROTATE_FROM_ATTACK:
			if (rotateAngle == 0.0f)
			{
				this->action = STANDING;
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
				this->action = STANDING;
			}
			else
			{
				this->deltaFinalRotate = deltaRotate + std::abs(distanceRotated);
				this->referenceFrame.rotateY(deltaFinalRotate);
				this->rotating = ROTATE_TO_ATTACK;
				this->action = STANDING;
			}
			break;
		}
		break;
	}
}

void Soldier::render()
{
	this->torso->referenceFrame = this->referenceFrame;
	this->frameStack.setBaseFrame(this->torso->referenceFrame);
	this->torso->render();
	this->frameStack.push();
	{
		this->frameStack.translate(0, 1, 0);
		this->frameStack.rotateY(degrees / 2);
		this->head->referenceFrame = this->frameStack.top();
		this->head->render();
	}
	this->frameStack.pop();
	this->frameStack.push();
	{
		this->frameStack.translate(-0.75f, 0.5f, 0);
		this->frameStack.rotateX(this->armdegrees);
		this->shoulderR->referenceFrame = this->frameStack.top();
		this->shoulderR->render();
		this->frameStack.push();
		{
			this->frameStack.translate(0, -0.625f, 0);
			this->armR->referenceFrame = this->frameStack.top();
			this->armR->render();
			this->frameStack.push();
			{
				this->frameStack.translate(0, -0.375f, 0.25f);
				this->gun->referenceFrame = this->frameStack.top();
				this->gun->render();
				this->frameStack.push();
				{
					this->frameStack.translate(0, -shotDistance, 0);
					this->laser->referenceFrame = this->frameStack.top();
					this->laser->render();
				}
				this->frameStack.pop();
			}
			this->frameStack.pop();
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();
	this->frameStack.push();
	{
		this->frameStack.translate(0.75f, 0.5f, 0);
		this->frameStack.rotateX(-degrees);
		this->shoulderL->referenceFrame = this->frameStack.top();
		this->shoulderL->render();
		this->frameStack.push();
		{
			this->frameStack.translate(0, -0.625f, 0);
			this->armL->referenceFrame = this->frameStack.top();
			this->armL->render();
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();
	this->frameStack.push();
	{
		this->frameStack.translate(0.3125f, -1.0625, 0);
		this->frameStack.translate(0, 0.3125, 0);
		this->frameStack.rotateX(degrees);
		this->frameStack.translate(0, -0.3125, 0);
		this->legL->referenceFrame = this->frameStack.top();
		this->legL->render();
		this->frameStack.push();
		{
			this->frameStack.translate(0, -0.5f, 0.1875f);
			this->footL->referenceFrame = this->frameStack.top();
			this->footL->render();
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();
	this->frameStack.push();
	{
		this->frameStack.translate(-0.3125f, -1.0625, 0);
		this->frameStack.translate(0, 0.3125, 0);
		this->frameStack.rotateX(-degrees);
		this->frameStack.translate(0, -0.3125, 0);
		this->legR->referenceFrame = this->frameStack.top();
		this->legR->render();
		this->frameStack.push();
		{
			this->frameStack.translate(0, -0.5f, 0.1875f);
			this->footR->referenceFrame = this->frameStack.top();
			this->footR->render();
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();
}

void Soldier::fixLocation(float x, float z)
{
	this->referenceFrame.setPosition(x, -3.235f, z);
}