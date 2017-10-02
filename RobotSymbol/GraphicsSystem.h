#pragma once
#ifndef GRAPHICS_SYSTEM
#define GRAPHICS_SYSTEM

#include "BaseObject.h"
#include "RGBA.h"

#include <string>
using std::string;

class ShaderManager;
class GameWorld;
class ViewingFrustum;
class TheGame;
class GameObject;
class TextureManager;

class GraphicsSystem : public BaseObject
{
protected:
	ShaderManager *shaderManager;

	GameWorld *gameWorld;

	ViewingFrustum *frustum;

	TheGame* theGame;

	TextureManager* textureManager;

	bool initialized;

	float tickLengthSeconds;

	float totalElapsedSeconds;

public:
	GraphicsSystem(
		ShaderManager *shaderManager, 
		GameWorld *gameWorld, 
		ViewingFrustum *frustum,
		TextureManager* textureManager);

	virtual ~GraphicsSystem();

	void setLogger(Logger* logger);

	virtual void setTheGame(TheGame* theGame) { this->theGame = theGame; }

	virtual ShaderManager *getShaderManager() { return this->shaderManager; }

	virtual GameWorld *getGameWorld() { return this->gameWorld; }

	virtual ViewingFrustum *getViewingFrustum() { return this->frustum; }

	virtual TextureManager* getTextureManager() { return this->textureManager; }

	virtual GameObject* getGameObject(const string& name) = 0;

	virtual bool initialize() = 0;

	virtual void setUpViewingEnvironment() = 0;

	virtual void clearViewPort(
		unsigned int left, unsigned int top, 
		unsigned int width, unsigned int height, 
		RGBA color) = 0;

	virtual void update(float elapsedSeconds);

	virtual void render() = 0;

	virtual void sendViewingParametersToGPU() {}

	bool isInitialized() const { return this->initialized; }

	// Param:
	// frameRate - the number of frames per second e.g. 50
	void setFrameRate(unsigned int frameRate);

	void setLastTickSeconds(float lastTickSeconds) { 
		this->totalElapsedSeconds = lastTickSeconds; 
	}
};

#endif

