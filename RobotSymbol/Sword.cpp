#include "Sword.h"
#include "Cuboid.h"
#include "Axis.h"

Sword::Sword(const string& name) : OGL3DCompositeObject(name)
{
	this->grip = new Cuboid("grip", 0.1f, 0.1f, 0.5f, { 1, 1, 0, 1 });
	this->guard = new Cuboid("guard", 0.5f, 0.1f, 0.1f, { 1, 0, 0, 1 });
	this->blade = new Cuboid("blade", 0.1f, 0.01f, 2.0f);
	this->blade->material.shininess = 100;
	this->blade->material.specular = { 1, 1, 1, 1 };
	this->axis = new Axis("Axes", 2.0f);
}

Sword::~Sword()
{
}

void Sword::setShaderProgram(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
	this->grip->setShaderProgram(this->shaderProgram);
	this->guard->setShaderProgram(this->shaderProgram);
	this->blade->setShaderProgram(this->shaderProgram);
	this->axis->setShaderProgram(this->shaderProgram);
}

void Sword::update(float elapsedSeconds)
{
	OGL3DCompositeObject::update(elapsedSeconds);
}

void Sword::render()
{
	this->grip->referenceFrame = this->referenceFrame;
	this->frameStack.setBaseFrame(this->grip->referenceFrame);
	this->grip->render();
	this->frameStack.push();
	{
		this->frameStack.translate(0, -0.3f, 0);
		this->guard->referenceFrame = this->frameStack.top();
		this->guard->render();
		this->frameStack.push();
		{
			this->frameStack.translate(0, -1.0f, 0);
			this->axis->render(this->frameStack.top());
			this->blade->referenceFrame = this->frameStack.top();
			this->blade->render();
		}
		this->frameStack.pop();
	}
	this->frameStack.pop();
}
