#include "Turret.h"
#include "Cuboid.h"

Turret::Turret(const string& name) :
	OGL3DCompositeObject(name)
{
	this->base = new Cuboid("base", 2, 2, 2, { 0, 0, 1, 1 });
	this->torso = new Cuboid("torso", 1, 1, 1);
	this->turret = new Cuboid("turret", 1.5, 0.5, 0.5, { 1, 0, 1, 1 });
	this->angleY = 0;
	this->angleZ = 0;
	this->speedZ = 60;
}

Turret::~Turret()
{
	delete this->torso;
	delete this->base;
	delete this->turret;
}

void Turret::setShaderProgram(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
	this->base->setShaderProgram(this->shaderProgram);
	this->torso->setShaderProgram(this->shaderProgram);
	this->turret->setShaderProgram(this->shaderProgram);
}

void Turret::update(float elapsedSeconds)
{
	OGL3DCompositeObject::update(elapsedSeconds);

	this->angleY += (30 * elapsedSeconds);
	if (this->angleY > 360) this->angleY -= 360;
	this->angleZ += (this->speedZ * elapsedSeconds);
	if (this->angleZ > 45) {
		this->speedZ = -60;
	}
	else if (angleZ < -45) {
		this->speedZ = 60;
	}
}

void Turret::render()
{
	this->base->referenceFrame = this->referenceFrame;
	this->frameStack.setBaseFrame(this->base->referenceFrame);
	this->base->render();
	this->frameStack.push();
	{
		this->frameStack.translate(0, 1.5f, 0);
		this->frameStack.rotateY(angleY);
		this->torso->render(this->frameStack.top());
		this->frameStack.push();
		{
			this->frameStack.translate(0, 0, 0.75f);
			this->frameStack.rotateY(90);
			this->frameStack.rotateZ(angleZ);
			this->frameStack.translate(-0.4f, 0, 0);
			this->turret->render(frameStack.top());
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();
}
