#include "GameEngine.h"
#include "Logger.h"
#include "GameWindow.h"
#include "GraphicsSystem.h"
#include "CoreSystem.h"
#include "PCInputSystem.h"
#include "GameWorld.h"
#include "ITimer.h"
#include "GameObjectManager.h"
#include "ShaderManager.h"
#include "TheGame.h"
#include "AssetLoader.h"

GameEngine::GameEngine(
	Logger *logger,
	TheGame *theGame,
	AssetLoader* assetLoader,
	CoreSystem *core,
	GraphicsSystem *graphics, 
	GameWindow *window,
	PCInputSystem *inputSystem,
	ITimer *timer) : BaseObject(logger)
{
	this->core = core;
	this->core->setLogger(this->logger);

	this->theGame = theGame;
	this->theGame->setLogger(this->logger);
	this->theGame->gameEngine = this;

	this->assetLoader = assetLoader;
	this->assetLoader->setGameEngine(this);
	this->assetLoader->setLogger(this->logger);

	this->graphics = graphics;
	this->graphics->setTheGame(this->theGame);
	this->graphics->setLogger(this->logger);

	this->window = window;
	this->window->setLogger(this->logger);
	this->window->setGameEngine(this);

	this->inputSystem = inputSystem;
	this->graphics->getGameWorld()->setInputSystem(this->inputSystem);
	this->graphics->getGameWorld()->setTheGame(this->theGame);

	this->timer = timer;

	this->initialized = false;
}

GameEngine::~GameEngine()
{
	delete this->logger;
	delete this->core;
	delete this->graphics;
	delete this->window;
	delete this->inputSystem;
	delete this->timer;
	delete this->theGame;
}

void GameEngine::initializeWindowAndGraphics(const string& uniformAssetFilename)
{
	// First create the window
	this->initialized = this->window->create();
	if (this->initialized) {
		// Next, intialize the graphics system
		this->initialized = this->graphics->initialize();
		// Set up the GPU
		if (this->initialized) {
			setupGraphicsParameters(uniformAssetFilename);
			this->graphics->setUpViewingEnvironment();
		}
	}
}

void GameEngine::loadShaders(const string& shaderAssetFilename)
{
	this->theGame->loadShaders(shaderAssetFilename);
}

void GameEngine::setupGraphicsParameters(const string& uniformAssetFilename)
{
	this->theGame->setupGraphicsParameters(uniformAssetFilename);
}

void GameEngine::setupGame(const string& gameAssetsFilename)
{
	this->theGame->setup(gameAssetsFilename);
}

void GameEngine::run()
{
	if (this->initialized){
		this->window->show();
		this->window->listenForEvents(this->timer);
	}
	else {
		if (this->logger) {
			this->logger->log("The engine was not initialized!");
		}
	}
}


