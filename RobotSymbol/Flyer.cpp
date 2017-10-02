#include "Flyer.h"
#include "TexturedCuboid.h"
#include "OGLTexturedFlatSurface.h"
#include "IBehavior.h"

/// <summary>
/// Initializes a new instance of the <see cref="Flyer"/> class.
/// </summary>
/// <param name="name">The name.</param>
Flyer::Flyer(const string& name) :
	OGL3DCompositeObject(name)
{
	this->reader = new TextFileReader();
	this->base = new TexturedCuboid("base", reader->GetWidth("Flyer.txt", "base"),
											reader->GetDepth("Flyer.txt", "base"),
											reader->GetHeight("Flyer.txt", "base"),
											reader->GetRGBA("Flyer.txt", "base"));
	this->leftArm = new TexturedCuboid("leftArm", reader->GetWidth("Flyer.txt", "leftArm"),
												  reader->GetDepth("Flyer.txt", "leftArm"),
												  reader->GetHeight("Flyer.txt", "leftArm"),
												  reader->GetRGBA("Flyer.txt", "leftArm"));
	this->leftHand = new TexturedCuboid("leftHand", reader->GetWidth("Flyer.txt", "leftHand"),
													reader->GetDepth("Flyer.txt", "leftHand"),
													reader->GetHeight("Flyer.txt", "leftHand"),
													reader->GetRGBA("Flyer.txt", "leftHand"));
	this->rightArm = new TexturedCuboid("rightArm", reader->GetWidth("Flyer.txt", "rightArm"),
													reader->GetDepth("Flyer.txt", "rightArm"),
													reader->GetHeight("Flyer.txt", "rightArm"),
													reader->GetRGBA("Flyer.txt", "rightArm"));
	this->rightHand = new TexturedCuboid("rightHand", reader->GetWidth("Flyer.txt", "rightHand"),
													reader->GetDepth("Flyer.txt", "rightHand"),
													reader->GetHeight("Flyer.txt", "rightHand"),
													reader->GetRGBA("Flyer.txt", "rightHand"));
	this->body = new TexturedCuboid("body", reader->GetWidth("Flyer.txt", "body"),
											reader->GetDepth("Flyer.txt", "body"),
											reader->GetHeight("Flyer.txt", "body"),
											reader->GetRGBA("Flyer.txt", "body"));
	this->chest = new TexturedCuboid("chest", reader->GetWidth("Flyer.txt", "chest"),
											  reader->GetDepth("Flyer.txt", "chest"),
											  reader->GetHeight("Flyer.txt", "chest"),
											  reader->GetRGBA("Flyer.txt", "chest"));
	this->head = new TexturedCuboid("head", reader->GetWidth("Flyer.txt", "head"),
											reader->GetDepth("Flyer.txt", "head"),
											reader->GetHeight("Flyer.txt", "head"),
											reader->GetRGBA("Flyer.txt", "head"));
	this->laser = new TexturedCuboid("laser", reader->GetWidth("Flyer.txt", "laser"),
											  reader->GetDepth("Flyer.txt", "laser"),
											  reader->GetHeight("Flyer.txt", "laser"),
											  reader->GetRGBA("Flyer.txt", "laser"));
	this->face = new OGLTexturedFlatSurface("lid", reader->GetWidth("Flyer.txt", "face"),
												   reader->GetHeight("Flyer.txt", "face"), 
												   1, 1, reader->GetRGBA("Flyer.txt", "face"));
}


/// <summary>
/// Finalizes an instance of the <see cref="Flyer"/> class.
/// </summary>
Flyer::~Flyer()
{
	delete this->base;
	delete this->leftArm;
	delete this->leftHand;
	delete this->rightArm;
	delete this->rightHand;
	delete this->body;
	delete this->chest;
	delete this->head;
	delete this->laser;
	delete this->face;
}

/// <summary>
/// Sets the shader program.
/// </summary>
/// <param name="shaderProgram">The shader program.</param>
void Flyer::setShaderProgram(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
	this->base->setShaderProgram(this->shaderProgram);
	this->leftArm->setShaderProgram(this->shaderProgram);
	this->leftHand->setShaderProgram(this->shaderProgram);
	this->body->setShaderProgram(this->shaderProgram);
	this->chest->setShaderProgram(this->shaderProgram);
	this->head->setShaderProgram(this->shaderProgram);
	this->laser->setShaderProgram(this->shaderProgram);
	this->rightArm->setShaderProgram(this->shaderProgram);
	this->rightHand->setShaderProgram(this->shaderProgram);
	this->face->setShaderProgram(this->shaderProgram);
}

/// <summary>
/// Sets the robot texture.
/// </summary>
/// <param name="t">The texture.</param>
void Flyer::setRobotTexture(Texture *t)
{
	this->base->setTexture(t);
	this->leftArm->setTexture(t);
	this->leftHand->setTexture(t);
	this->rightArm->setTexture(t);
	this->rightHand->setTexture(t);
	this->body->setTexture(t);
	this->chest->setTexture(t);
	this->head->setTexture(t);
}

string Flyer::getState()
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

/// <summary>
/// Sets the laser texture.
/// </summary>
/// <param name="t">The texture.</param>
void Flyer::setLaserTexture(Texture *t)
{
	this->laser->setTexture(t);

}

/// <summary>
/// Updates the specified elapsed seconds.
/// </summary>
/// <param name="elapsedSeconds">The elapsed seconds.</param>
void Flyer::update(float elapsedSeconds)
{
	switch (this->action)
	{
	case STANDING:
		break;

	case MOVING:
		switch (moving)
		{
		case TAKE_OFF:
			if (this->flyRotateAngle < 60.0f)
			{
				this->flyRotateAngle += (60.0f * elapsedSeconds);
			}
			if (this->flyTakeOffDistance < 2.0f)
			{
				this->flyTakeOffDistance += (2.0f *elapsedSeconds);
				this->referenceFrame.translate(0, 0, -2.0f * elapsedSeconds);
			}
			if (this->flyTakeOffDistance >= 2.0f) {
				this->moving = FLYING;
			}
			break;

		case FLYING:
			this->flyDistance += (moveDistance * elapsedSeconds);
			this->referenceFrame.translate(0, moveDistance * elapsedSeconds, 0);
			if (this->flyDistance > moveDistance) {
				this->moving = LANDING;
			}
			break;

		case LANDING:
			if (this->flyRotateAngle > 0.0f)
			{
				this->flyRotateAngle -= (60.0f * elapsedSeconds);
			}
			if (this->flyTakeOffDistance > 0.0f)
			{
				this->flyTakeOffDistance -= (2.0f *elapsedSeconds);
				this->referenceFrame.translate(0, 0, 2.0f * elapsedSeconds);
			}
			if (this->flyRotateAngle <= 0.0f && this->flyTakeOffDistance <= 0.0f) {
				flyRotateAngle = 0.0f;
				flyTakeOffDistance = 0.0f;
				this->moving = TAKE_OFF;
				this->action = ROTATING;
			}
			break;

		}
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
				this->referenceFrame.rotateZ(deltaRotate);
			}
			else if (deltaRotate > 0)
			{
				this->deltaFinalRotate = deltaRotate - (distanceRotated - std::abs(rotateAngle));
				this->referenceFrame.rotateZ(deltaFinalRotate);
				this->distanceRotated = std::abs(rotateAngle);
				this->rotating = ROTATE_FROM_MOVE;
				this->action = MOVING;
			}
			else
			{
				this->deltaFinalRotate = deltaRotate + (distanceRotated - std::abs(rotateAngle));
				this->referenceFrame.rotateZ(deltaFinalRotate);
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
				this->referenceFrame.rotateZ(-deltaRotate);
			}
			else if (deltaRotate > 0)
			{
				this->deltaFinalRotate = deltaRotate - std::abs(distanceRotated);
				this->referenceFrame.rotateZ(-deltaFinalRotate);
				fixLocation(posX, posZ);
				this->rotating = ROTATE_TO_MOVE;
				this->action = STANDING;
			}
			else
			{
				this->deltaFinalRotate = deltaRotate + std::abs(distanceRotated);
				this->referenceFrame.rotateZ(-deltaFinalRotate);
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
				this->referenceFrame.rotateZ(deltaRotate);
			}
			else if (deltaRotate > 0)
			{
				this->deltaFinalRotate = deltaRotate - (distanceRotated - std::abs(rotateAngle));
				this->referenceFrame.rotateZ(deltaFinalRotate);
				this->distanceRotated = std::abs(rotateAngle);
				this->rotating = ROTATE_FROM_ATTACK;
				this->action = ATTACKING;
			}
			else
			{
				this->deltaFinalRotate = deltaRotate + (distanceRotated - std::abs(rotateAngle));
				this->referenceFrame.rotateZ(deltaFinalRotate);
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
				this->referenceFrame.rotateZ(-deltaRotate);
			}
			else if (deltaRotate > 0)
			{
				this->deltaFinalRotate = deltaRotate - std::abs(distanceRotated);
				this->referenceFrame.rotateZ(-deltaFinalRotate);
				this->rotating = ROTATE_TO_ATTACK;
				this->action = STANDING;
			}
			else
			{
				this->deltaFinalRotate = deltaRotate + std::abs(distanceRotated);
				this->referenceFrame.rotateZ(-deltaFinalRotate);
				this->rotating = ROTATE_TO_ATTACK;
				this->action = STANDING;
			}
			break;
		}
		break;

	case DYING:
		switch (dying)
		{
		case SPINNING:
			if (this->dyingRotateAngle < 1800.0f)
			{
				this->dyingRotateAngle += (720.0f * elapsedSeconds);
				this->referenceFrame.rotateZ(720.0f * elapsedSeconds);
			}
			if (this->dyingRotateAngle >= 1800.0f) {
				this->dying = EXPLODING;
			}
			break;

		case EXPLODING:
			this->dyingDistance += (25.0f * elapsedSeconds);
			this->referenceFrame.moveForward(25.0f * elapsedSeconds);
			if (this->dyingDistance > 50.0f) {
				this->action = STANDING;
			}
			break;
		}
		break;

	case ATTACKING:
		switch (attacking)
		{
		case AIMING:
			if (this->attackRotateAngle > -75.0f)
			{
				this->attackRotateAngle -= (90.0f * elapsedSeconds);
			}
			if (this->attackRotateAngle <= -75.0f) {
				this->attacking = SHOOTING;
			}
			break;

		case SHOOTING:
			this->attackDistance += (6.0f * elapsedSeconds);
			if (this->attackDistance > 2.0f) {
				attackDistance = 0.0f;
				this->attacking = HOLSTERING;
			}
			break;

		case HOLSTERING:
			if (this->attackRotateAngle < 0.0f)
			{
				this->attackRotateAngle += (90.0f * elapsedSeconds);
			}
			if (this->attackRotateAngle >= 0.0f) {
				attackRotateAngle = 0.0f;
				this->attacking = AIMING;
				this->action = ROTATING;
			}
			break;
		}
		break;
	} // end switch
}

/// <summary>
/// Renders this instance.
/// </summary>
void Flyer::render()
{
	this->base->referenceFrame = this->referenceFrame;
	this->frameStack.setBaseFrame(this->base->referenceFrame);
	this->base->render();
	this->frameStack.push();
	{
		this->frameStack.translate(0, 0, -1.0f);
		this->body->referenceFrame = this->frameStack.top();
		this->body->render();
		this->frameStack.push();
		{
			this->frameStack.translate(0, 0, -1.0f);
			this->chest->referenceFrame = this->frameStack.top();
			this->chest->render();
			this->frameStack.push();
			{
				this->frameStack.translate(0, 0, -.5f);
				this->head->referenceFrame = this->frameStack.top();
				this->head->render();
				this->frameStack.push();
				{
					this->frameStack.translate(0, .26f, 0);
					this->face->referenceFrame = this->frameStack.top();
					this->face->render();
				}
				this->frameStack.pop();
				this->frameStack.push();
				{
					this->frameStack.translate(1.75f, 0.0f, .75f);
					if (this->action == MOVING)
						this->frameStack.rotateX(flyRotateAngle);
					this->frameStack.translate(-1.0f, 0.0f, 0.0f);
					this->leftArm->referenceFrame = this->frameStack.top();
					this->leftArm->render();
					this->frameStack.push();
					{
						this->frameStack.translate(0, 0, .625f);
						this->leftHand->referenceFrame = this->frameStack.top();
						this->leftHand->render();
						this->frameStack.push();
						{
							this->frameStack.pop();
							this->frameStack.translate(-.5f, 0, -.625f);
							if (this->action == ATTACKING)
								this->frameStack.rotateX(attackRotateAngle);
							this->frameStack.translate(-1.0f, 0, 0);
							this->rightArm->referenceFrame = this->frameStack.top();
							this->rightArm->render();
							this->frameStack.push();
							{
								this->frameStack.translate(0, 0, .625f);
								this->rightHand->referenceFrame = this->frameStack.top();
								this->rightHand->render();
								this->frameStack.push();
								{
									this->frameStack.translate(0, 0, -.5f);
									if (this->action == ATTACKING)
										this->frameStack.translate(0, 0, attackDistance);
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
				}
				this->frameStack.pop();
			}
			this->frameStack.pop();
		}
		this->frameStack.pop();
	}
}

void Flyer::fixLocation(float x, float z)
{
	this->referenceFrame.setPosition(x, -4.9f, z);
}