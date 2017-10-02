#pragma once
#ifndef OGL_GRAPHICS_SYSTEM
#define OGL_GRAPHICS_SYSTEM

#include "GraphicsSystem.h"

#include <gl\glew.h>
#include <string>
using std::string;

class OGLShaderManager;
class OGLVertexShader;
class OGLFragmentShader;
class OGLShaderProgram;
class GameWorld;
class OGLViewingFrustum;
class OGLCamera;
class GameObject;
class OGL2DTextDisplay;
class TextureManager;
class OGL2DHUD;

class OGLGraphicsSystem : public GraphicsSystem
{
protected:
	OGLShaderManager *oglShaderManager;
	OGLViewingFrustum *oglFrustum;
	OGLCamera *oglCamera;
	OGL2DTextDisplay *textDisplay;
	OGL2DHUD *playerHUD, *enemyHUD, *alertHUD;

public:
	OGLGraphicsSystem(
		OGLShaderManager *shaderManager, 
		GameWorld *gameWorld, 
		OGLViewingFrustum *oglFrustum,
		TextureManager *textureManager);

	virtual ~OGLGraphicsSystem();

	OGLShaderManager *getOGLShaderManager() { 
		return (OGLShaderManager*)this->shaderManager; }

	void addVertexShader(
		const string& name, 
		OGLVertexShader *shader, 
		const string& sourceCode = "");

	void addFragmentShader(
		const string& name, 
		OGLFragmentShader *shader, 
		const string& sourceCode = "");

	void addShaderProgram(
		const string& name, 
		OGLShaderProgram *shaderProgram, 
		const string& vertexShaderName, 
		const string& fragmentShaderName);

	GameObject* getGameObject(const string& name);

	void setTextDisplay(OGL2DTextDisplay *textDisplay) { this->textDisplay = textDisplay; }

	OGL2DTextDisplay* getTextDisplay() { return this->textDisplay; }

	void setPlayerHUD(OGL2DHUD *playerHUD) { this->playerHUD = playerHUD; }

	OGL2DHUD* getPlayerHUD() { return this->playerHUD; }

	void setEnemyHUD(OGL2DHUD *enemyHUD) { this->enemyHUD = enemyHUD; }

	OGL2DHUD* getEnemyHUD() { return this->enemyHUD; }

	bool initialize();

	void setUpViewingEnvironment();

	void clearViewPort(
		unsigned int left, unsigned int top,
		unsigned int width, unsigned int height,
		RGBA color);

	void update(float elapsedSeconds);

	void render();

	void setObjectsShaderProgram(const string& shaderProgramName);

	void setObjectShaderProgram(const string& objectName, const string& shaderProgramName);
};

#endif

