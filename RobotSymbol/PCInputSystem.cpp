#include <Windows.h>
#include "PCInputSystem.h"


PCInputSystem::PCInputSystem(void)
{
	this->resetKeys();
	this->resetMouse();
}

PCInputSystem::~PCInputSystem(void)
{
}

void PCInputSystem::resetKeys()
{
	for (int i = 0; i < sizeof(this->keys); i++) {
		this->keys[i] = false;
	}
}

void PCInputSystem::resetMouse()
{
	this->mouseX = 0;
	this->mouseY = 0;
	this->oldMouseX = 0;
	this->oldMouseY = 0;
	this->dirX = 0;
	this->dirY = 0;
}

void PCInputSystem::setMousePosition(unsigned short mouseX, unsigned short mouseY) 
{
	this->oldMouseX = this->mouseX;
	this->oldMouseY = this->mouseY;
	this->mouseX = mouseX;
	this->mouseY = mouseY;
	this->dirX = this->mouseX - this->oldMouseX;
	this->dirY = this->mouseY - this->oldMouseY;
}

