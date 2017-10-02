#pragma once
#ifndef GAME_ENGINE
#define GAME_ENGINE

#include "BaseObject.h"

#include <string>
using std::string;

class TextFileReader;
class OGLShaderManager;
class OGLVertexShader;
class OGLFragmentShader;
class Renderer;
class GameWindow;
class OGLShaderProgram;
class GraphicsSystem;
class CoreSystem;
class PCInputSystem;
class ITimer;
class AssetLoader;
class TheGame;
class AssetLoader;

class GameEngine : public BaseObject
{
protected:
	CoreSystem *core;
	GraphicsSystem *graphics;
	GameWindow *window;
	PCInputSystem *inputSystem;
	ITimer *timer;
	TheGame *theGame;
	AssetLoader* assetLoader;
	bool initialized;

public:
	GameEngine(
		Logger *logger, 
		TheGame *theGame,
		AssetLoader* assetLoader,
		CoreSystem *core, 
		GraphicsSystem *graphics, 
		GameWindow *window,
		PCInputSystem *inputSystem,
		ITimer *timer);

	virtual ~GameEngine();

	GraphicsSystem *getGraphicsSystem() { return this->graphics; }

	TheGame *getTheGame() { return this->theGame; }

	AssetLoader* getAssetLoader() { return this->assetLoader; }

	CoreSystem *getCoreSystem() { return this->core; }

	PCInputSystem *getInputSystem() { return this->inputSystem; }

	virtual void loadShaders(const string& shaderAssetFilename);

	virtual void initializeWindowAndGraphics(const string& uniformAssetFilename);

	virtual void setupGraphicsParameters(const string& uniformAssetFilename);

	virtual void setupGame(const string& gameAssetsFilename="");
	
	void run();
};

#endif

