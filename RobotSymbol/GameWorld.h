#pragma once
#ifndef GAME_WORLD
#define GAME_WORLD

#include "BaseObject.h"

#include <vector>
using std::vector;

class GameObjectManager;
class Camera;
class PCInputSystem;
class LightSource;
class TheGame;

class GameWorld : public BaseObject
{
protected:
	GameObjectManager *objectManager;
	Camera *camera;
	PCInputSystem *inputSystem;
	TheGame* theGame;

public:
	vector<LightSource *> localLights;

public:
	GameWorld(
		GameObjectManager *objectManager, Camera *camera);

	virtual ~GameWorld();

	GameObjectManager *getObjectManager() { return this->objectManager; }

	Camera *getCamera() { return this->camera; }

	void setTheGame(TheGame* theGame) { this->theGame = theGame; }

	void setInputSystem(PCInputSystem* inputSystem) { this->inputSystem = inputSystem; }

	virtual void update(float elapsedSeconds);

	virtual void processInputs();

protected:
	
};

#endif

