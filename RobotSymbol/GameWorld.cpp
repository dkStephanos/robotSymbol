#include <Windows.h>
#include "GameWorld.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "PCInputSystem.h"
#include "OGLFirstPersonCamera.h"
#include "LightSource.h"
#include "TheGame.h"

GameWorld::GameWorld(
	GameObjectManager *objectManager, Camera *camera) : BaseObject(nullptr)
{
	this->objectManager = objectManager;
	this->camera = camera;
	this->inputSystem = nullptr;
}

GameWorld::~GameWorld()
{
	delete this->objectManager;
	delete this->camera;
	for (size_t i = 0; i < this->localLights.size(); i++) {
		delete this->localLights[i];
	}
}

void GameWorld::update(float elapsedSeconds)
{
	for (size_t i = 0; i < this->localLights.size(); i++) {
		this->localLights[i]->update(elapsedSeconds);
	}
	this->objectManager->update(elapsedSeconds);
	this->objectManager->updateVisibleObjects();
}

void GameWorld::processInputs()
{
	this->theGame->processInputs();
}
