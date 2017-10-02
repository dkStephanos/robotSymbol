#include "OGLGraphicsSystem.h"
#include "OGLShaderManager.h"
#include "OGLVertexShader.h"
#include "OGLFragmentShader.h"
#include "OGLShaderProgram.h"
#include "OGL2DHUD.h"
#include "GameWorld.h"
#include "OGLViewingFrustum.h"
#include "Logger.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "OGLObject.h"
#include "LightSource.h"
#include "MathUtil.h"
#include "TheGame.h"
#include "OGL2DTextDisplay.h"
#include "TextureManager.h"
#include "OGL3DObject.h"

#include <map>
using std::map;
#include <glm\gtc\type_ptr.hpp>
#include <glutil\glutil.h>

OGLGraphicsSystem::OGLGraphicsSystem(
	OGLShaderManager *shaderManager, 
	GameWorld *gameWorld, 
	OGLViewingFrustum *oglFrustum,
	TextureManager *textureManager) :
		GraphicsSystem(
			shaderManager, gameWorld, oglFrustum, textureManager)
{
	this->oglShaderManager = (OGLShaderManager*)this->shaderManager;
	this->oglFrustum = (OGLViewingFrustum*)this->frustum;
	this->oglCamera = (OGLCamera*)this->gameWorld->getCamera();
	this->textDisplay = nullptr;
	this->playerHUD = nullptr;
	this->enemyHUD = nullptr;
}

OGLGraphicsSystem::~OGLGraphicsSystem()
{
}

void OGLGraphicsSystem::addVertexShader(
	const string& name, OGLVertexShader *shader, const string& sourceCode)
{
	if (sourceCode != "") {
		shader->setSource(sourceCode);
	}
	shader->setLogger(this->logger);
	this->oglShaderManager->addVertexShader(name, shader);
}

void OGLGraphicsSystem::addFragmentShader(
	const string& name, OGLFragmentShader *shader, const string& sourceCode)
{
	if (sourceCode != "") {
		shader->setSource(sourceCode);
	}
	shader->setLogger(this->logger);
	this->oglShaderManager->addFragmentShader(name, shader);
}

void OGLGraphicsSystem::addShaderProgram(
	const string& name,
	OGLShaderProgram *shaderProgram, const string& vertexShaderName, const string& fragmentShaderName)
{
	shaderProgram->setLogger(this->logger);
	this->oglShaderManager->addShaderProgram(
		name, shaderProgram, vertexShaderName, fragmentShaderName);
}

GameObject* OGLGraphicsSystem::getGameObject(const string& name)
{
	return getGameWorld()->getObjectManager()->getObject(name);
}

bool OGLGraphicsSystem::initialize()
{
	this->initialized = this->oglShaderManager->createShaders();
	return this->initialized;
}

void OGLGraphicsSystem::setUpViewingEnvironment()
{
	this->theGame->setupViewingEnvironment();
}

void OGLGraphicsSystem::clearViewPort(
	unsigned int left, unsigned int top,
	unsigned int width, unsigned int height,
	RGBA color)
{
	glViewport(left, top, width, height);
	glClearColor(color.red, color.green, color.blue, color.alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OGLGraphicsSystem::setObjectsShaderProgram(const string& shaderProgramName)
{
	GLuint shaderProgram = 
		this->oglShaderManager->getShaderHandle(shaderProgramName);
	auto allObjects = this->getGameWorld()->getObjectManager()->getAllObjects();

	OGLObject *object;
	auto iterator = allObjects.begin();
	while (iterator != allObjects.end()) {
		object = (OGLObject*)iterator->second;
		object->setShaderProgram(shaderProgram);
		iterator++;
	}
}

void OGLGraphicsSystem::setObjectShaderProgram(const string& objectName, const string& shaderProgramName)
{
	GLuint shaderProgram =
		this->oglShaderManager->getShaderHandle(shaderProgramName);
	GameObjectManager *objectMgr = this->gameWorld->getObjectManager();
	OGLObject *object = (OGLObject *)objectMgr->getObject(objectName);
	object->setShaderProgram(shaderProgram);
}

void OGLGraphicsSystem::update(float elapsedSeconds)
{
	GraphicsSystem::update(elapsedSeconds);

	this->theGame->updateTextDisplay();
}


void OGLGraphicsSystem::render()
{
	this->theGame->sendShaderData();

	GameObjectManager *objectMgr = this->gameWorld->getObjectManager();
	objectMgr->renderVisibleObjects();

	if (this->textDisplay) {
		this->textDisplay->render();
	}

	if (this->playerHUD) {
		this->playerHUD->render();
	}

	if (this->enemyHUD) {
		this->enemyHUD->render();
	}
}



