#include <Windows.h>
#include "TheGame.h"
#include "OGLShaderManager.h"
#include "GameEngine.h"
#include "OGLShaderManager.h"
#include "OGLGraphicsSystem.h"
#include "OGL2DHUD.h"
#include "MountainRange.h"
#include "Diamond.h"
#include "OGLViewingFrustum.h"
#include "GameWorld.h"
#include "LightSource.h"
#include "Camera.h"
#include "SpaceShuttle.h"
#include "CoreSystem.h"
#include "OGLVertexShader.h"
#include "TextFileReader.h"
#include "OGLFragmentShader.h"
#include "AssetLoader.h"
#include "OGLFirstPersonCamera.h"
#include "OGLObject.h"
#include "RotateYBehavior.h"
#include "GameObjectManager.h"
#include "PCInputSystem.h"
#include "BackForthBehavior.h"
#include "OGL3DObject.h"
#include "OGL2DTextDisplay.h"
#include "BMPTextureLoader.h"
#include "OGL2DTexture.h"
#include "Plane.h"
#include "CustomTexture.h"
#include "Soldier.h"
#include "TexturedCuboid.h"
#include "TextureManager.h"
#include "GameAssetLoader.h"
#include "LineBox.h"
#include "Sphere.h"
#include "Pyramid.h"
#include "OGLTexturedFlatSurface.h"
#include "Construct.h"
#include "Axis.h"
#include "Logger.h"
#include "GridSurface.h"
#include "Flyer.h"
#include <string>
#include <iostream>
#include <sstream>
#include <gl\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <ctime>
#include "Soldier.h"
#include "Tank.h"
#include "ShakeBehavior.h"

TheGame::TheGame() : BaseObject(nullptr)
{
	srand((unsigned int)time(NULL));
}

TheGame::~TheGame()
{
}

void TheGame::loadShaders(const string& shaderAssetFilename)
{
	GameAssetLoader* loader = (GameAssetLoader*)this->gameEngine->getAssetLoader();
	loader->loadAssetFile(shaderAssetFilename);
	loader->loadOnlyTheShaders();
}

void TheGame::setupGraphicsParameters(const string& uniformAssetFilename)
{
	GameAssetLoader* loader = (GameAssetLoader*)this->gameEngine->getAssetLoader();
	loader->loadAssetFile(uniformAssetFilename);
	loader->loadOnlyTheUniforms();
}

void TheGame::setupViewingEnvironment()
{
	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	OGLViewingFrustum* frustum = (OGLViewingFrustum*)graphics->getViewingFrustum();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram* shader = 
		(OGLShaderProgram*)shaderMgr->getShader("ShaderProgramIllumination");
	shader->setUniformData(
		"cameraToScreenMatrix",
		(void*)glm::value_ptr(frustum->getMatrix()));
	shader->sendData("cameraToScreenMatrix");

	shader = (OGLShaderProgram*)shaderMgr->getShader("ShaderProgram3d");
	shader->setUniformData(
		"cameraToScreenMatrix",
		(void*)glm::value_ptr(frustum->getMatrix()));
	shader->sendData("cameraToScreenMatrix");

	shader = (OGLShaderProgram*)shaderMgr->getShader("TextureShader");
	shader->setUniformData(
		"cameraToScreenMatrix",
		(void*)glm::value_ptr(frustum->getMatrix()));
	shader->sendData("cameraToScreenMatrix");

	// Cull back faces and use counter-clockwise winding of front faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
}

void TheGame::sendShaderData()
{
	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	OGLViewingFrustum* frustum = (OGLViewingFrustum*)graphics->getViewingFrustum();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram* shader =
		(OGLShaderProgram*)shaderMgr->getShader("ShaderProgramIllumination");

	glm::vec3 globalLightPos = glm::vec3(100, 100, 0);
	shader->setUniformData(
		"worldGlobalLightPos",
		(void*)glm::value_ptr(globalLightPos));
	shader->sendData("worldGlobalLightPos");

	float globalLightIntensity = 0.0f;
	shader->setUniformData(
		"globalLightIntensity",
		(void*)&globalLightIntensity);
	shader->sendData("globalLightIntensity");

	GameWorld* gameWorld = graphics->getGameWorld();

	int numberOfLights = (int)gameWorld->localLights.size();
	shader->setUniformData(
		"numberOfLights",
		(void*)&numberOfLights);
	shader->sendData("numberOfLights");

	glm::vec3 lightPositions[10];
	float lightIntensities[10];
	for (int i = 0; i < numberOfLights; i++) {
		lightPositions[i] = gameWorld->localLights[i]->getPosition();
		lightIntensities[i] = gameWorld->localLights[i]->getIntensity();
	}
	shader->setUniformData(
		"lightPositions",
		(void*)lightPositions);
	shader->sendData("lightPositions");

	shader->setUniformData(
		"lightIntensities",
		(void*)lightIntensities);
	shader->sendData("lightIntensities");

	Camera *camera = (Camera*)gameWorld->getCamera();

	shader->setUniformData(
		"worldToCameraMatrix",
		(void*)glm::value_ptr(camera->orientation));
	shader->sendData("worldToCameraMatrix");

	shader->setUniformData(
		"worldCameraPos",
		(void*)glm::value_ptr(glm::vec3(camera->orientation[3])));
	shader->sendData("worldCameraPos");

	// Texture Shader
	shader = (OGLShaderProgram*)shaderMgr->getShader("TextureShader");

	shader->setUniformData(
		"worldGlobalLightPos",
		(void*)glm::value_ptr(globalLightPos));
	shader->sendData("worldGlobalLightPos");

	shader->setUniformData(
		"globalLightIntensity",
		(void*)&globalLightIntensity);
	shader->sendData("globalLightIntensity");

	shader->setUniformData(
		"numberOfLights",
		(void*)&numberOfLights);
	shader->sendData("numberOfLights");

	shader->setUniformData(
		"lightPositions",
		(void*)lightPositions);
	shader->sendData("lightPositions");

	shader->setUniformData(
		"lightIntensities",
		(void*)lightIntensities);
	shader->sendData("lightIntensities");

	shader->setUniformData(
		"worldToCameraMatrix",
		(void*)glm::value_ptr(camera->orientation));
	shader->sendData("worldToCameraMatrix");

	shader->setUniformData(
		"worldCameraPos",
		(void*)glm::value_ptr(glm::vec3(camera->orientation[3])));
	shader->sendData("worldCameraPos");

	shader = (OGLShaderProgram*)shaderMgr->getShader("ShaderProgram3d");

	shader->setUniformData(
		"worldToCameraMatrix",
		(void*)glm::value_ptr(camera->orientation));
	shader->sendData("worldToCameraMatrix");

	shader->setUniformData(
		"worldCameraPos",
		(void*)glm::value_ptr(glm::vec3(camera->orientation[3])));
	shader->sendData("worldCameraPos");
}

void TheGame::setup(const string& gameAssetFilename)
{
	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	TextureManager* texMgr = graphics->getTextureManager();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();

	OGLShaderProgram* plainShader3d = (OGLShaderProgram*)shaderMgr->getShader("ShaderProgram3d");
	OGLShaderProgram* textShader = (OGLShaderProgram*)shaderMgr->getShader("TextShader");

	AssetLoader* loader = this->gameEngine->getAssetLoader();
	this->logger->log("Loading assets...");
	loader->loadAssetFile(gameAssetFilename);
	loader->loadAssets();
	this->logger->log("...Done loading assets.");

	OGLFirstPersonCamera *camera =
		(OGLFirstPersonCamera *)graphics->getGameWorld()->getCamera();
	camera->setPosition(0.0f, 5.0f, -10.0f);

	OGL2DTexture* texture = (OGL2DTexture*)texMgr->getTexture("Letters");;
	OGL2DTextDisplay* textDisplay = new OGL2DTextDisplay();
	textDisplay->setTexture(texture);
	textDisplay->setShaderProgram(textShader->getHandle());
	graphics->setTextDisplay(textDisplay);

	OGL2DTexture* texturePlayerHUD = (OGL2DTexture*)texMgr->getTexture("PlayerTex");
	OGL2DHUD* playerHUD = new OGL2DHUD("PlayerHud", -1.5f, -.5f);
	playerHUD->setTexture(texturePlayerHUD);
	playerHUD->setShaderProgram(textShader->getHandle());
	playerHUD->setTextDisplayTexture(texture);
	playerHUD->setTextDisplayShader(textShader);
	graphics->setPlayerHUD(playerHUD);

	OGL2DTexture* textureEnemyHUD = (OGL2DTexture*)texMgr->getTexture("EnemyTex");
	OGL2DHUD* enemyHUD = new OGL2DHUD("EnemyHud", 0.5f, 1.5f);
	enemyHUD->setTexture(textureEnemyHUD);
	enemyHUD->setShaderProgram(textShader->getHandle());
	enemyHUD->setTextDisplayTexture(texture);
	enemyHUD->setTextDisplayShader(textShader);
	graphics->setEnemyHUD(enemyHUD);

	OGL2DTexture* texGrass = (OGL2DTexture*)texMgr->getTexture("GrassTex");

	OGLObject* object = (OGL3DObject*)graphics->getGameObject("LosePlat");
	if (object) {
		object->referenceFrame.translate(1, -4.89, -1);
	}

	object = (OGL3DObject*)graphics->getGameObject("WinPlat");
	if (object) {
		object->referenceFrame.translate(1, -4.89, -1);
	}

	object = (OGL3DObject*)graphics->getGameObject("Floor");
	if (object) {
		object->setGround(true);
	}

	float distanceX = 10;
	float distanceZ = -10;
	int counter = 0;
	int x = 0;
	int z = 0;
	for (z = 0; z < 10; z++)
	{
		for (x = 0; x < 10; x++)
		{
			string name = "Grid";
			std::stringstream ss;
			ss << counter;
			name = name + ss.str();
			object = (OGL3DObject*)graphics->getGameObject(name);
			if (object) {
				object->referenceFrame.setPosition(distanceX, -4.9, distanceZ);
				object->setVisibility(true);
			}
			distanceX -= 2.00;
			counter++;
		}
		distanceZ += 2.00;
		distanceX = 10;
	}

	GridSurface* tile;
	Soldier* sobject = (Soldier*)graphics->getGameObject("PlayerSoldier");
	if (sobject) {
		tile = (GridSurface*)graphics->getGameObject("Grid11");
		sobject->setVisibility(true);
		sobject->referenceFrame.setPosition(tile->referenceFrame.getPosition().x, -3.235, tile->referenceFrame.getPosition().z);
		sobject->setPosition(2, 2);
		tile->setPlayer();
		tile->setSoldier();
	}

	Tank* tobject = (Tank*)graphics->getGameObject("PlayerTank");
	if (tobject) {
		tile = (GridSurface*)graphics->getGameObject("Grid18");
		tobject->setVisibility(true);
		tobject->referenceFrame.setPosition(tile->referenceFrame.getPosition().x, -3.175, tile->referenceFrame.getPosition().z);
		tobject->setPosition(9, 2);
		tile->setPlayer();
		tile->setTank();
	}

	Flyer* fobject = (Flyer*)graphics->getGameObject("PlayerFlyer");
	if (fobject) {
		tile = (GridSurface*)graphics->getGameObject("Grid13");
		fobject->setVisibility(true);
		fobject->referenceFrame.setPosition(tile->referenceFrame.getPosition().x, -4.9, tile->referenceFrame.getPosition().z);
		fobject->setPosition(4, 2);
		fobject->referenceFrame.rotateX(90);
		tile->setPlayer();
		tile->setFlyer();
	}

	sobject = (Soldier*)graphics->getGameObject("EnemySoldier");
	if (sobject) {
		tile = (GridSurface*)graphics->getGameObject("Grid84");
		sobject->setVisibility(true);
		sobject->referenceFrame.setPosition(tile->referenceFrame.getPosition().x, -3.235, tile->referenceFrame.getPosition().z);
		sobject->setPosition(5, 9);
		sobject->referenceFrame.rotateY(180);
		tile->setEnemy();
		tile->setSoldier();
	}

	tobject = (Tank*)graphics->getGameObject("EnemyTank");
	if (tobject) {
		tile = (GridSurface*)graphics->getGameObject("Grid50");
		tobject->setVisibility(true);
		tobject->referenceFrame.setPosition(tile->referenceFrame.getPosition().x, -3.175, tile->referenceFrame.getPosition().z);
		tobject->setPosition(1, 6);
		tobject->referenceFrame.rotateY(180);
		tile->setEnemy();
		tile->setTank();
	}

	fobject = (Flyer*)graphics->getGameObject("EnemyFlyer");
	if (fobject) {
		tile = (GridSurface*)graphics->getGameObject("Grid46");
		fobject->setVisibility(true);
		fobject->referenceFrame.setPosition(tile->referenceFrame.getPosition().x, -4.9, tile->referenceFrame.getPosition().z);
		fobject->setPosition(7, 5);
		fobject->referenceFrame.rotateX(90);
		fobject->referenceFrame.rotateZ(180);
		tile->setEnemy();
		tile->setFlyer();
	}

	cursorX = ((Soldier*)graphics->getGameObject("PlayerSoldier"))->getXPos();
	cursorZ = ((Soldier*)graphics->getGameObject("PlayerSoldier"))->getZPos();

	object = new SpaceShuttle("SpaceShuttle1", 1.25f);
	object->referenceFrame.setPosition(-19, -4.5, -19);
	object->referenceFrame.rotateY(-90.0f);
	object->material.shininess = 200;
	object->material.specular = { 1, 1, 1, 1 };
	object->setShaderProgram(plainShader3d->getHandle());
	objectMgr->addObject(object->getName(), object);
	object->setVisibility(true);

	object = new SpaceShuttle("SpaceShuttle2", 1.25f);
	object->referenceFrame.setPosition(19, -4.5, 19);
	object->referenceFrame.rotateY(90.0f);
	object->material.shininess = 200;
	object->material.specular = { 1, 1, 1, 1 };
	object->setShaderProgram(plainShader3d->getHandle());
	objectMgr->addObject(object->getName(), object);
	object->setVisibility(true);

	object = new MountainRange("MountainRange1", 1.6f);
	object->referenceFrame.setPosition(22, -3.4, 0);
	object->referenceFrame.rotateY(-90.0f);
	object->setShaderProgram(plainShader3d->getHandle());
	objectMgr->addObject(object->getName(), object);
	object->setVisibility(true);

	object = new MountainRange("MountainRange2", 1.6f);
	object->referenceFrame.setPosition(-22, -3.4, 0);
	object->referenceFrame.rotateY(90.0f);
	object->setShaderProgram(plainShader3d->getHandle());
	objectMgr->addObject(object->getName(), object);
	object->setVisibility(true);

	object = new MountainRange("MountainRange3", 1.4f);
	object->referenceFrame.setPosition(8, -3.6, -22);
	object->setShaderProgram(plainShader3d->getHandle());
	objectMgr->addObject(object->getName(), object);
	object->setVisibility(true);

	object = new MountainRange("MountainRange4", 1.4f);
	object->referenceFrame.setPosition(8, -3.6, -22);
	object->referenceFrame.rotateWorldY(180.0f);
	object->setShaderProgram(plainShader3d->getHandle());
	objectMgr->addObject(object->getName(), object);
	object->setVisibility(true);

	object = new MountainRange("MountainRange5", 1.2f);
	object->referenceFrame.setPosition(-6, -3.8, -22);
	object->setShaderProgram(plainShader3d->getHandle());
	objectMgr->addObject(object->getName(), object);
	object->setVisibility(true);

	object = new MountainRange("MountainRange6", 1.2f);
	object->referenceFrame.setPosition(-6, -3.8, -22);
	object->referenceFrame.rotateWorldY(180.0f);
	object->setShaderProgram(plainShader3d->getHandle());
	objectMgr->addObject(object->getName(), object);
	object->setVisibility(true);

	OGLShaderProgram* shader = (OGLShaderProgram*)shaderMgr->getShader("TextureShader");
	texture = (OGL2DTexture*)texMgr->getTexture("DiamondTex");
	object = new Diamond("Diamond1", 2.0f);
	object->referenceFrame.setPosition(-21, -1.25, 19);
	object->setTexture(texture);
	object->material.shininess = 200;
	object->material.specular = { 1, 1, 1, 1 };
	object->setBehavior(new RotateYBehavior(object, 50.5f));
	object->setShaderProgram(shader->getHandle());
	objectMgr->addObject(object->getName(), object);
	object->setVisibility(true);
	
	object = new Diamond("Diamond2", 2.0f);
	object->referenceFrame.setPosition(21, -1.25, -19);
	object->setTexture(texture);
	object->material.shininess = 200;
	object->material.specular = { 1, 1, 1, 1 };
	object->setBehavior(new RotateYBehavior(object, 50.5f));
	object->setShaderProgram(shader->getHandle());
	objectMgr->addObject(object->getName(), object);
	object->setVisibility(true);
}


void TheGame::updateTextDisplay()
{
	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	OGL2DTextDisplay* textDisplay = graphics->getTextDisplay();
	Flyer* pFlyer = (Flyer*)graphics->getGameObject("PlayerFlyer");
	Flyer* eFlyer = (Flyer*)graphics->getGameObject("EnemyFlyer");
	Soldier* pSoldier = (Soldier*)graphics->getGameObject("PlayerSoldier");
	Soldier* eSoldier = (Soldier*)graphics->getGameObject("EnemySoldier");
	Tank* pTank = (Tank*)graphics->getGameObject("PlayerTank");
	Tank* eTank = (Tank*)graphics->getGameObject("EnemyTank");

	textDisplay->clear();
	textDisplay->addText("Robot Symbol", -1, 1);
	textDisplay->addText(std::to_string(pFlyer->getHealth()), -.61, -.773);
	textDisplay->addText(std::to_string(eFlyer->getHealth()), .895, .71);
	textDisplay->addText(std::to_string(pSoldier->getHealth()), -.915, -.773);
	textDisplay->addText(std::to_string(eSoldier->getHealth()), .595, .72);
	textDisplay->addText(std::to_string(pTank->getHealth()), -.78, -.773);
	textDisplay->addText(std::to_string(eTank->getHealth()), .73, .72);
}

void TheGame::processInputs()
{
	OGLGraphicsSystem* graphics = 
		(OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	OGLFirstPersonCamera *camera = 
		(OGLFirstPersonCamera *)graphics->getGameWorld()->getCamera();
	PCInputSystem* inputSystem = this->gameEngine->getInputSystem();
	OGL3DObject* object;

	Soldier* playerSoldier = (Soldier*)graphics->getGameObject("PlayerSoldier");
	Tank* playerTank = (Tank*)graphics->getGameObject("PlayerTank"); 
	Flyer* playerFlyer = (Flyer*)graphics->getGameObject("PlayerFlyer");
	Soldier* enemySoldier = (Soldier*)graphics->getGameObject("EnemySoldier");
	Tank* enemyTank = (Tank*)graphics->getGameObject("EnemyTank");
	Flyer* enemyFlyer = (Flyer*)graphics->getGameObject("EnemyFlyer");
	string name;
	GridSurface* tile;
	bool OnSameSpace = false;

	float rotateAngle;
	float distance;

	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	TextureManager* texMgr = graphics->getTextureManager();
	OGL2DTexture* texture = (OGL2DTexture*)texMgr->getTexture("RobotSymbolTex");
	OGL2DTexture* texGrass = (OGL2DTexture*)texMgr->getTexture("GrassTex");
	OGL2DTexture* texNorm = (OGL2DTexture*)texMgr->getTexture("GridTex");
	OGL2DTexture* texRed = (OGL2DTexture*)texMgr->getTexture("RedTex");

	camera->setJustLooking();
	camera->setNotMoving();

	if (inputSystem->keys[VK_LEFT]) {
		camera->setLookingLeft();
	}
	else if (inputSystem->keys[VK_RIGHT]) {
		camera->setLookingRight();
	}

	if (inputSystem->keys[VK_UP]) {
		camera->setLookingUp();
	}
	else if (inputSystem->keys[VK_DOWN]) {
		camera->setLookingDown();
	}

	if (inputSystem->keys['T'] && cursorOn)
	{
		if (!TPressed)
		{
			this->TPressed = true;

			if (this->cursorZ < 10 && !showingRange && !showingAttackRange)
			{
				setColor(this->cursorX, this->cursorZ, texNorm);
				setColor(this->cursorX, this->cursorZ + 1, texture);
				this->cursorZ++;
			}
			else if (this->cursorZ < 10 && showingRange)
			{
				if (determineDistance(cursorX, pressedPosX, cursorZ + 1, pressedPosZ) < 3)
				{
					setColor(this->cursorX, this->cursorZ, texGrass);
					setColor(this->cursorX, this->cursorZ + 1, texture);
					this->cursorZ++;
				}
			}
			else if (this->cursorZ < 10 && showingAttackRange)
			{
				if (determineDistance(cursorX, pressedPosX, cursorZ + 1, pressedPosZ) < 2)
				{
					setColor(this->cursorX, this->cursorZ, texRed);
					setColor(this->cursorX, this->cursorZ + 1, texture);
					this->cursorZ++;
				}
			}
		}
	}
	else
	{
		this->TPressed = false;
	}

	if (inputSystem->keys['G'] && cursorOn)
	{
		if (!GPressed)
		{
			GPressed = true;

			if (this->cursorZ > 1 && !showingRange && !showingAttackRange)
			{
				setColor(this->cursorX, this->cursorZ, texNorm);
				setColor(this->cursorX, this->cursorZ - 1, texture);
				this->cursorZ--;
			}
			else if (this->cursorZ > 1 && showingRange)
			{
				if (determineDistance(cursorX, pressedPosX, cursorZ - 1, pressedPosZ) < 3)
				{
					setColor(this->cursorX, this->cursorZ, texGrass);
					setColor(this->cursorX, this->cursorZ - 1, texture);
					this->cursorZ--;
				}
			}
			else if (this->cursorZ > 1 && showingAttackRange)
			{
				if (determineDistance(cursorX, pressedPosX, cursorZ - 1, pressedPosZ) < 2)
				{
					setColor(this->cursorX, this->cursorZ, texRed);
					setColor(this->cursorX, this->cursorZ - 1, texture);
					this->cursorZ--;
				}
			}
		}
	}
	else
	{
		this->GPressed = false;
	}

	if (inputSystem->keys['H'] && cursorOn)
	{
		if(!HPressed)
		{
			HPressed = true;

			if (this->cursorX < 10 && !showingRange && !showingAttackRange)
			{
				setColor(this->cursorX, this->cursorZ, texNorm);
				setColor(this->cursorX + 1, this->cursorZ, texture);
				this->cursorX++;
			}
			else if (this->cursorX < 10 && showingRange)
			{
				if (determineDistance(cursorX + 1, pressedPosX, cursorZ, pressedPosZ) < 3)
				{
					setColor(this->cursorX, this->cursorZ, texGrass);
					setColor(this->cursorX + 1, this->cursorZ, texture);
					this->cursorX++;
				}
			}
			else if (this->cursorX < 10 && showingAttackRange)
			{
				if (determineDistance(cursorX + 1, pressedPosX, cursorZ, pressedPosZ) < 2)
				{
					setColor(this->cursorX, this->cursorZ, texRed);
					setColor(this->cursorX + 1, this->cursorZ, texture);
					this->cursorX++;
				}
			}
		}
	}
	else
	{
		this->HPressed = false;
	}

	if (inputSystem->keys['F'] && cursorOn)
	{
		if (!FPressed)
		{
			FPressed = true;

			if (this->cursorX > 1 && !showingRange && !showingAttackRange)
			{
				setColor(this->cursorX, this->cursorZ, texNorm);
				setColor(this->cursorX - 1, this->cursorZ, texture);
				this->cursorX--;
			}
			else if (this->cursorX > 1 && showingRange)
			{
				if (determineDistance(cursorX - 1, pressedPosX, cursorZ, pressedPosZ) < 3)
				{
					setColor(this->cursorX, this->cursorZ, texGrass);
					setColor(this->cursorX - 1, this->cursorZ, texture);
					this->cursorX--;
				}
			}
			else if (this->cursorX > 1 && showingAttackRange)
			{
				if (determineDistance(cursorX - 1, pressedPosX, cursorZ, pressedPosZ) < 2)
				{
					setColor(this->cursorX, this->cursorZ, texRed);
					setColor(this->cursorX - 1, this->cursorZ, texture);
					this->cursorX--;
				}
			}
		}
	}
	else
	{
		this->FPressed = false;
	}

	if (inputSystem->keys['P'] && cursorOn)
	{
		if (!PPressed)
		{
			PPressed = true;

			if (!showingRange)
			{
				showMoveRange(this->cursorX, this->cursorZ, texGrass);
				showingRange = true;
				this->pressedPosX = this->cursorX;
				this->pressedPosZ = this->cursorZ;
			}
			else
			{
				showMoveRange(this->pressedPosX, this->pressedPosZ, texNorm);
				setColor(this->pressedPosX, this->pressedPosZ, texNorm);
				setColor(this->cursorX, this->cursorZ, texture);
				showingRange = false;

				Soldier* sobject = (Soldier*)graphics->getGameObject("Soldier");
				if (sobject) {
					string name = determineGridName(cursorX, cursorZ);
					sobject->referenceFrame.setPosition(graphics->getGameObject(name)->referenceFrame.getPosition().x, -3.235, graphics->getGameObject(name)->referenceFrame.getPosition().z);
					sobject->setPosition(cursorX, cursorZ);
					sobject->setMoving();
				}
			}
		}
	}
	else
	{
		this->PPressed = false;
	}

	if (inputSystem->keys['O'])
	{
		if (!OPressed)
		{	//remember to check your privilege
			OPressed = true; 

			if (cursorOn)
			{
				cursorOn = false;
				setColor(cursorX, cursorZ, texNorm);
			}
			else
			{
				cursorOn = true;
				setColor(cursorX, cursorZ, texture);
			}
		}
	}
	else
	{
		OPressed = false;
	}

	if (inputSystem->keys['Z'])
	{
		if (!ZPressed)
		{
			ZPressed = true;
			switch (this->state)
			{
			case PLAYER_TURN_START:
				this->state = PLAYER_SOLDIER_MOVE;	//Display "PLAYER TURN" message here in the final game
				break;

			case PLAYER_SOLDIER_MOVE:
				if (playerSoldier->getLiving())
				{
					this->cursorOn = true;
					this->cursorX = playerSoldier->getXPos();
					this->cursorZ = playerSoldier->getZPos();
					setColor(cursorX, cursorZ, texture);
					showMoveRange(cursorX, cursorZ, texGrass);
					showingRange = true;
					this->pressedPosX = this->cursorX;
					this->pressedPosZ = this->cursorZ;
					this->state = PLAYER_SOLDIER_ATTACKCHECK;
				}
				else
				{
					if (playerTank->getLiving())
						state = PLAYER_TANK_MOVE;
					else if (playerFlyer->getLiving())
						state = PLAYER_FLYER_MOVE;
					else
						state = PLAYER_LOSE;
				}
				break;

			case PLAYER_SOLDIER_ATTACKCHECK:
				name = determineGridName(cursorX, cursorZ);
				tile = (GridSurface*)graphics->getGameObject(name);
				OnSameSpace = false;

				if (cursorX == playerSoldier->getXPos() && cursorZ == playerSoldier->getZPos())
					OnSameSpace = true;
				
				if (tile->containsNothing() || OnSameSpace)
				{
					showMoveRange(pressedPosX, pressedPosZ, texNorm);
					name = determineGridName(pressedPosX, pressedPosZ);
					tile = (GridSurface*)graphics->getGameObject(name);
					tile->setEmpty();
					setColor(this->pressedPosX, this->pressedPosZ, texNorm);
					showingRange = false;
					this->pressedPosX = this->cursorX;
					this->pressedPosZ = this->cursorZ;
					if (playerSoldier) {
						string name = determineGridName(cursorX, cursorZ);
						tile = (GridSurface*)graphics->getGameObject(name);
						tile->setPlayer();
						tile->setSoldier();
						float rotateAngle = determineDirection(this->cursorX, this->cursorZ, playerSoldier->getXPos(), playerSoldier->getZPos());
						float distance = determineMoveScale(this->cursorX, this->cursorZ, playerSoldier->getXPos(), playerSoldier->getZPos());
						playerSoldier->setPosXZ(tile->referenceFrame.getPosition().x, tile->referenceFrame.getPosition().z);
						playerSoldier->setRotateAngle(rotateAngle);
						playerSoldier->setMoveDistance(distance);
						playerSoldier->setPosition(cursorX, cursorZ);
						if (!OnSameSpace)
							playerSoldier->setRotatingToMove();


					}
					showAttackRange(cursorX, cursorZ, texRed);
					setColor(this->cursorX, this->cursorZ, texture);
					showingAttackRange = true;
					this->state = PLAYER_SOLDIER_BATTLE;
				}
				break;

			case PLAYER_SOLDIER_BATTLE: 
				if (playerSoldier->getState() == "STANDING")
				{
					name = determineGridName(cursorX, cursorZ);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsEnemy())
					{
						showAttackRange(pressedPosX, pressedPosZ, texNorm);
						setColor(this->pressedPosX, this->pressedPosZ, texNorm);
						showingAttackRange = false;
						this->cursorOn = false;

						string enemyType = tile->getContainsType();
						if (enemyType == "SOLDIER")
						{
							float rotateAngle = determineDirection(this->cursorX, this->cursorZ, playerSoldier->getXPos(), playerSoldier->getZPos());
							playerSoldier->setRotateAngle(rotateAngle);
							playerSoldier->setRotatingToAttack();
							int hp = enemySoldier->getHealth();
							hp -= playerSoldier->getPower();
							enemySoldier->setHealth(hp);
							if (enemySoldier->getHealth() <= 0)
							{
								enemySoldier->setHealth(0);
								enemySoldier->setBehavior(new ShakeBehavior(enemySoldier));
								enemySoldier->setDying();
								enemySoldier->setLiving(false);
								tile->setEmpty();
								if (!enemyFlyer->getLiving() && !enemyTank->getLiving())
									state = PLAYER_WIN;
							}
						}
						else if (enemyType == "TANK")
						{
							float rotateAngle = determineDirection(this->cursorX, this->cursorZ, playerSoldier->getXPos(), playerSoldier->getZPos());
							playerSoldier->setRotateAngle(rotateAngle);
							playerSoldier->setRotatingToAttack();
							int hp = enemyTank->getHealth();
							hp -= playerSoldier->getPower();
							enemyTank->setHealth(hp);
							if (enemyTank->getHealth() <= 0)
							{
								enemyTank->setHealth(0);
								enemyTank->setDying();
								enemyTank->setLiving(false);
								tile->setEmpty();
								if (!enemySoldier->getLiving() && !enemyFlyer->getLiving())
									state = PLAYER_WIN;
							}
						}
						else if (enemyType == "FLYER")
						{
							float rotateAngle = determineDirection(this->cursorX, this->cursorZ, playerSoldier->getXPos(), playerSoldier->getZPos());
							playerSoldier->setRotateAngle(rotateAngle);
							playerSoldier->setRotatingToAttack();
							int hp = enemyFlyer->getHealth();
							hp -= playerSoldier->getPower();
							enemyFlyer->setHealth(hp);
							if (enemyFlyer->getHealth() <= 0)
							{
								enemyFlyer->setHealth(0);
								enemyFlyer->setDying();
								enemyFlyer->setLiving(false);
								tile->setEmpty();
								if (!enemySoldier->getLiving() && !enemyTank->getLiving())
									state = PLAYER_WIN;
							}
						}

						if (state != PLAYER_WIN)
						{
							if (playerTank->getLiving())
								state = PLAYER_TANK_MOVE;
							else if (playerFlyer->getLiving())
								state = PLAYER_FLYER_MOVE;
							else if (playerSoldier->getLiving())
								state = ENEMY_TURN_START;
							else
								state = PLAYER_LOSE;
						}
					}
					else
					{
						showAttackRange(pressedPosX, pressedPosZ, texNorm);
						setColor(this->pressedPosX, this->pressedPosZ, texNorm);
						showingAttackRange = false;
						this->cursorOn = false;

						if (state != PLAYER_WIN)
						{
							if (playerTank->getLiving())
								state = PLAYER_TANK_MOVE;
							else if (playerFlyer->getLiving())
								state = PLAYER_FLYER_MOVE;
							else if (playerSoldier->getLiving())
								state = ENEMY_TURN_START;
							else
								state = PLAYER_LOSE;
						}
					}
				}
				break;

			case PLAYER_TANK_MOVE:
				this->cursorOn = true;
				this->cursorX = playerTank->getXPos();
				this->cursorZ = playerTank->getZPos();
				setColor(cursorX, cursorZ, texture);
				showMoveRange(cursorX, cursorZ, texGrass);
				showingRange = true;
				this->pressedPosX = this->cursorX;
				this->pressedPosZ = this->cursorZ;
				this->state = PLAYER_TANK_ATTACKCHECK;
				break;

			case PLAYER_TANK_ATTACKCHECK:
				name = determineGridName(cursorX, cursorZ);
				tile = (GridSurface*)graphics->getGameObject(name);
				OnSameSpace = false;

				if (cursorX == playerTank->getXPos() && cursorZ == playerTank->getZPos())
					OnSameSpace = true;

				if (tile->containsNothing() || OnSameSpace)
				{
					showMoveRange(pressedPosX, pressedPosZ, texNorm);
					name = determineGridName(pressedPosX, pressedPosZ);
					tile = (GridSurface*)graphics->getGameObject(name);
					tile->setEmpty();
					setColor(this->pressedPosX, this->pressedPosZ, texNorm);
					showingRange = false;
					this->pressedPosX = this->cursorX;
					this->pressedPosZ = this->cursorZ;
					if (playerTank) {
						string name = determineGridName(cursorX, cursorZ);
						tile = (GridSurface*)graphics->getGameObject(name);
						tile->setPlayer();
						tile->setTank();
						float rotateAngle = determineDirection(this->cursorX, this->cursorZ, playerTank->getXPos(), playerTank->getZPos());
						float distance = determineMoveScale(this->cursorX, this->cursorZ, playerTank->getXPos(), playerTank->getZPos());
						playerTank->setPosXZ(tile->referenceFrame.getPosition().x, tile->referenceFrame.getPosition().z);
						playerTank->setRotateAngle(rotateAngle);
						playerTank->setMoveDistance(distance);
						playerTank->setPosition(cursorX, cursorZ);
						if (!OnSameSpace)
							playerTank->setRotatingToMove();
					}
					showAttackRange(cursorX, cursorZ, texRed);
					setColor(this->cursorX, this->cursorZ, texture);
					showingAttackRange = true;
					this->state = PLAYER_TANK_BATTLE;
				}
				break;

			case PLAYER_TANK_BATTLE: //Will either go here or to PLAYER_FLYER_MOVE, depending on if the selected space has an enemy on it
				if (playerTank->getState() == "STANDING")
				{
					name = determineGridName(cursorX, cursorZ);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsEnemy())
					{
						showAttackRange(pressedPosX, pressedPosZ, texNorm);
						setColor(this->pressedPosX, this->pressedPosZ, texNorm);
						showingAttackRange = false;
						this->cursorOn = false;

						string enemyType = tile->getContainsType();
						if (enemyType == "SOLDIER")
						{
							float rotateAngle = determineDirection(this->cursorX, this->cursorZ, playerTank->getXPos(), playerTank->getZPos());
							playerTank->setRotateAngle(rotateAngle);
							playerTank->setRotatingToAttack();
							int hp = enemySoldier->getHealth();
							hp -= playerTank->getPower();
							enemySoldier->setHealth(hp);
							if (enemySoldier->getHealth() <= 0)
							{
								enemySoldier->setHealth(0);
								enemySoldier->setDying();
								enemySoldier->setBehavior(new ShakeBehavior(enemySoldier));
								enemySoldier->setLiving(false);
								tile->setEmpty();
								if (!enemyFlyer->getLiving() && !enemyTank->getLiving())
									state = PLAYER_WIN;
							}
						}
						else if (enemyType == "TANK")
						{
							float rotateAngle = determineDirection(this->cursorX, this->cursorZ, playerTank->getXPos(), playerTank->getZPos());
							playerTank->setRotateAngle(rotateAngle);
							playerTank->setRotatingToAttack();
							int hp = enemyTank->getHealth();
							hp -= playerTank->getPower();
							enemyTank->setHealth(hp);
							if (enemyTank->getHealth() <= 0)
							{
								enemyTank->setHealth(0);
								enemyTank->setDying();
								enemyTank->setLiving(false);
								tile->setEmpty();
								if (!enemySoldier->getLiving() && !enemyFlyer->getLiving())
									state = PLAYER_WIN;
							}

						}
						else if (enemyType == "FLYER")
						{
							float rotateAngle = determineDirection(this->cursorX, this->cursorZ, playerTank->getXPos(), playerTank->getZPos());
							playerTank->setRotateAngle(rotateAngle);
							playerTank->setRotatingToAttack();
							int hp = enemyFlyer->getHealth();
							hp -= playerTank->getPower();
							enemyFlyer->setHealth(hp);
							if (enemyFlyer->getHealth() <= 0)
							{
								enemyFlyer->setHealth(0);
								enemyFlyer->setDying();
								enemyFlyer->setLiving(false);
								tile->setEmpty();
								if (!enemySoldier->getLiving() && !enemyTank->getLiving())
									state = PLAYER_WIN;
							}
						}

						if (state != PLAYER_WIN)
						{
							if (playerFlyer->getLiving())
								state = PLAYER_FLYER_MOVE;
							else
								state = ENEMY_TURN_START;
						}
					}
					else
					{
						showAttackRange(pressedPosX, pressedPosZ, texNorm);
						setColor(this->pressedPosX, this->pressedPosZ, texNorm);
						showingAttackRange = false;
						this->cursorOn = false;

						if (state != PLAYER_WIN)
						{
							if (playerFlyer->getLiving())
								state = PLAYER_FLYER_MOVE;
							else
								state = ENEMY_TURN_START;
						}
					}

				}
				break;

			case (PLAYER_FLYER_MOVE) :
				this->cursorOn = true;
				this->cursorX = playerFlyer->getXPos();
				this->cursorZ = playerFlyer->getZPos();
				setColor(cursorX, cursorZ, texture);
				showMoveRange(cursorX, cursorZ, texGrass);
				showingRange = true;
				this->pressedPosX = this->cursorX;
				this->pressedPosZ = this->cursorZ;
				this->state = PLAYER_FLYER_ATTACKCHECK;
				break;

			case (PLAYER_FLYER_ATTACKCHECK) :
				name = determineGridName(cursorX, cursorZ);
				tile = (GridSurface*)graphics->getGameObject(name);
				OnSameSpace = false;

				if (cursorX == playerFlyer->getXPos() && cursorZ == playerFlyer->getZPos())
					OnSameSpace = true;

				if (tile->containsNothing() || OnSameSpace)
				{
					showMoveRange(pressedPosX, pressedPosZ, texNorm);
					name = determineGridName(pressedPosX, pressedPosZ);
					tile = (GridSurface*)graphics->getGameObject(name);
					tile->setEmpty();
					setColor(this->pressedPosX, this->pressedPosZ, texNorm);
					showingRange = false;
					this->pressedPosX = this->cursorX;
					this->pressedPosZ = this->cursorZ;
					if (playerFlyer) {
						string name = determineGridName(cursorX, cursorZ);
						tile = (GridSurface*)graphics->getGameObject(name);
						tile->setPlayer();
						tile->setFlyer();
						float rotateAngle = determineDirection(this->cursorX, this->cursorZ, playerFlyer->getXPos(), playerFlyer->getZPos());
						float distance = determineMoveScale(this->cursorX, this->cursorZ, playerFlyer->getXPos(), playerFlyer->getZPos());
						playerFlyer->setRotateAngle(rotateAngle);
						playerFlyer->setMoveDistance(distance);
						playerFlyer->setPosition(cursorX, cursorZ);
						playerFlyer->setPosXZ(tile->referenceFrame.getPosition().x, tile->referenceFrame.getPosition().z);

						if (!OnSameSpace)
							playerFlyer->setRotatingToMove();
					}
					showAttackRange(cursorX, cursorZ, texRed);
					setColor(this->cursorX, this->cursorZ, texture);
					showingAttackRange = true;
					this->state = PLAYER_FLYER_BATTLE;
				}
				break;
				
			case (PLAYER_FLYER_BATTLE) : 
				if (playerFlyer->getState() == "STANDING")
				{
					name = determineGridName(cursorX, cursorZ);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsEnemy())
					{
						showAttackRange(pressedPosX, pressedPosZ, texNorm);
						setColor(this->pressedPosX, this->pressedPosZ, texNorm);
						showingAttackRange = false;
						this->cursorOn = false;

						string enemyType = tile->getContainsType();
						if (enemyType == "SOLDIER")
						{
							float rotateAngle = determineDirection(this->cursorX, this->cursorZ, playerFlyer->getXPos(), playerFlyer->getZPos());
							playerFlyer->setRotateAngle(rotateAngle);
							playerFlyer->setRotatingToAttack();
							int hp = enemySoldier->getHealth();
							hp -= playerFlyer->getPower();
							enemySoldier->setHealth(hp);
							if (enemySoldier->getHealth() <= 0)
							{
								enemySoldier->setHealth(0);
								enemySoldier->setDying();
								enemySoldier->setBehavior(new ShakeBehavior(enemySoldier));
								enemySoldier->setLiving(false);
								tile->setEmpty();
								if (!enemyFlyer->getLiving() && !enemyTank->getLiving())
									state = PLAYER_WIN;
							}
						}
						else if (enemyType == "TANK")
						{
							float rotateAngle = determineDirection(this->cursorX, this->cursorZ, playerFlyer->getXPos(), playerFlyer->getZPos());
							playerFlyer->setRotateAngle(rotateAngle);
							playerFlyer->setRotatingToAttack();
							int hp = enemyTank->getHealth();
							hp -= playerFlyer->getPower();
							enemyTank->setHealth(hp);
							if (enemyTank->getHealth() <= 0)
							{
								enemyTank->setHealth(0);
								enemyTank->setDying();
								enemyTank->setLiving(false);
								tile->setEmpty();
								if (!enemySoldier->getLiving() && !enemyFlyer->getLiving())
									state = PLAYER_WIN;
							}

						}
						else if (enemyType == "FLYER")
						{
							float rotateAngle = determineDirection(this->cursorX, this->cursorZ, playerFlyer->getXPos(), playerFlyer->getZPos());
							playerFlyer->setRotateAngle(rotateAngle);
							playerFlyer->setRotatingToAttack();
							int hp = enemyFlyer->getHealth();
							hp -= playerFlyer->getPower();
							enemyFlyer->setHealth(hp);
							if (enemyFlyer->getHealth() <= 0)
							{
								enemyFlyer->setHealth(0);
								enemyFlyer->setDying();
								enemyFlyer->setLiving(false);
								tile->setEmpty();
								if (!enemySoldier->getLiving() && !enemyTank->getLiving())
									state = PLAYER_WIN;
							}
						}

						if (state != PLAYER_WIN)
							state = ENEMY_TURN_START;
					}
					else
					{
						showAttackRange(pressedPosX, pressedPosZ, texNorm);
						setColor(this->pressedPosX, this->pressedPosZ, texNorm);
						showingAttackRange = false;
						this->cursorOn = false;

						if (state != PLAYER_WIN)
							state = ENEMY_TURN_START;
					}
				}
				break;

			case (ENEMY_TURN_START) :
				if (enemySoldier->getLiving())
					state = ENEMY_SOLDIER_MOVE;
				else if (enemyTank->getLiving())
					state = ENEMY_TANK_MOVE;
				else if (enemyFlyer->getLiving())
					state = ENEMY_FLYER_MOVE;
				else
					state = PLAYER_WIN;
				break;

			case(ENEMY_SOLDIER_MOVE):
				int lowest, check;

				//MOVE SOLDIER
				if (playerSoldier->getLiving())
				{
					lowest = determineDistance(enemySoldier->getXPos(), playerSoldier->getXPos(), enemySoldier->getZPos(), playerSoldier->getZPos());
					lowestType = "S";
				}
				else
					lowest = 999;
				if (playerTank->getLiving())
				{
					check = determineDistance(enemySoldier->getXPos(), playerTank->getXPos(), enemySoldier->getZPos(), playerTank->getZPos());
					if (check < lowest)
					{
						lowest = check;
						lowestType = "T";
					}//if
					else if (check == lowest)
					{
						if (playerTank->getHealth() < playerSoldier->getHealth())
						{
							lowest = check;
							lowestType = "T";
						}//if
						else
						{
							res = rand() % 2;
							if (res == 0)				//randomly choose, 50% chance
							{
								lowest = check;
								lowestType = "T";
							}//if
						}//else
					}//elseif
				}//if
				if (playerFlyer->getLiving())
				{
					check = determineDistance(enemySoldier->getXPos(), playerFlyer->getXPos(), enemySoldier->getZPos(), playerFlyer->getZPos());
					if (check < lowest)
					{
						lowest = check;
						lowestType = "F";
					}
					else if (check == lowest)
					{
						if (lowestType == "S")
						{
							if (playerFlyer->getHealth() < playerSoldier->getHealth())
							{
								lowest = check;
								lowestType = "F";
							}
							else
							{
								res = rand() % 2;
								if (res == 0)				//randomly choose, 50% chance
								{
									lowest = check;
									lowestType = "F";
								}
							}
						}
						else if (lowestType == "T")
						{
							if (playerFlyer->getHealth() < playerTank->getHealth())
							{
								lowest = check;
								lowestType = "F";
							}
							else
							{
								res = rand() % 2;
								if (res == 0)				//randomly choose, 50% chance
								{
									lowest = check;
									lowestType = "F";
								}
							}
						}
					}
				}

				int targetX, targetZ;
				if (lowestType == "S")
				{
					targetX = playerSoldier->getXPos();
					targetZ = playerSoldier->getZPos();
				}
				if (lowestType == "T")
				{
					targetX = playerTank->getXPos();
					targetZ = playerTank->getZPos();
				}
				if (lowestType == "F")
				{
					targetX = playerFlyer->getXPos();
					targetZ = playerFlyer->getZPos();
				}

				this->logger->log("Enemy targeted:");
				this->logger->log(lowestType);
				this->logger->log("Distance:");
				this->logger->log(std::to_string(lowest));

				inRange = false;
				if ((lowest - 2) <= 1)
					inRange = true;

				lowest = 999;

				check = determineDistance(enemySoldier->getXPos(), targetX, enemySoldier->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					lowest = check;
					targetSpaceX = enemySoldier->getXPos();
					targetSpaceZ = enemySoldier->getZPos();
					OnSameSpace = true;
				}
				check = determineDistance(enemySoldier->getXPos()+2, targetX, enemySoldier->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemySoldier->getXPos() + 2, enemySoldier->getZPos());
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemySoldier->getXPos() + 2;
						targetSpaceZ = enemySoldier->getZPos();
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemySoldier->getXPos() + 1, targetX, enemySoldier->getZPos() + 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemySoldier->getXPos() + 1, enemySoldier->getZPos() + 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemySoldier->getXPos() + 1;
						targetSpaceZ = enemySoldier->getZPos() + 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemySoldier->getXPos() + 1, targetX, enemySoldier->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemySoldier->getXPos() + 1, enemySoldier->getZPos());
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemySoldier->getXPos() + 1;
						targetSpaceZ = enemySoldier->getZPos();
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemySoldier->getXPos() + 1, targetX, enemySoldier->getZPos() - 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemySoldier->getXPos() + 1, enemySoldier->getZPos() - 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemySoldier->getXPos() + 1;
						targetSpaceZ = enemySoldier->getZPos() - 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemySoldier->getXPos(), targetX, enemySoldier->getZPos() + 2, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemySoldier->getXPos(), enemySoldier->getZPos() + 2);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemySoldier->getXPos();
						targetSpaceZ = enemySoldier->getZPos() + 2;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemySoldier->getXPos(), targetX, enemySoldier->getZPos() + 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemySoldier->getXPos(), enemySoldier->getZPos() + 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemySoldier->getXPos();
						targetSpaceZ = enemySoldier->getZPos() + 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemySoldier->getXPos(), targetX, enemySoldier->getZPos() - 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemySoldier->getXPos(), enemySoldier->getZPos() - 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemySoldier->getXPos();
						targetSpaceZ = enemySoldier->getZPos() - 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemySoldier->getXPos(), targetX, enemySoldier->getZPos() - 2, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemySoldier->getXPos(), enemySoldier->getZPos() - 2);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemySoldier->getXPos();
						targetSpaceZ = enemySoldier->getZPos() - 2;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemySoldier->getXPos() - 2, targetX, enemySoldier->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemySoldier->getXPos() - 2, enemySoldier->getZPos());
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemySoldier->getXPos() - 2;
						targetSpaceZ = enemySoldier->getZPos();
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemySoldier->getXPos() - 1, targetX, enemySoldier->getZPos() + 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemySoldier->getXPos() - 1, enemySoldier->getZPos() + 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemySoldier->getXPos() - 1;
						targetSpaceZ = enemySoldier->getZPos() + 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemySoldier->getXPos() - 1, targetX, enemySoldier->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemySoldier->getXPos() - 1, enemySoldier->getZPos());
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemySoldier->getXPos() - 1;
						targetSpaceZ = enemySoldier->getZPos();
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemySoldier->getXPos() - 1, targetX, enemySoldier->getZPos() - 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemySoldier->getXPos() - 1, enemySoldier->getZPos() - 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemySoldier->getXPos() - 1;
						targetSpaceZ = enemySoldier->getZPos() - 1;
						OnSameSpace = false;
					}
				}

				name = determineGridName(enemySoldier->getXPos(), enemySoldier->getZPos());
				tile = (GridSurface*)graphics->getGameObject(name);
				tile->setEmpty();
				name = determineGridName(targetSpaceX, targetSpaceZ);
				tile = (GridSurface*)graphics->getGameObject(name);
				tile->setEnemy();
				tile->setSoldier();
				rotateAngle = determineDirectionEnemy(targetSpaceX, targetSpaceZ, enemySoldier->getXPos(), enemySoldier->getZPos());
				distance = determineMoveScale(targetSpaceX, targetSpaceZ, enemySoldier->getXPos(), enemySoldier->getZPos());
				enemySoldier->setPosXZ(tile->referenceFrame.getPosition().x, tile->referenceFrame.getPosition().z);
				enemySoldier->setRotateAngle(rotateAngle);
				enemySoldier->setMoveDistance(distance);
				enemySoldier->setPosition(targetSpaceX, targetSpaceZ);
				if (!OnSameSpace)
					enemySoldier->setRotatingToMove();
				
				if (inRange)
					state = ENEMY_SOLDIER_ATTACK;
				else if (enemyTank->getLiving())
					state = ENEMY_TANK_MOVE;
				else if (enemyFlyer->getLiving())
					state = ENEMY_FLYER_MOVE;
				else
					state = PLAYER_TURN_START;

				break;

			case (ENEMY_SOLDIER_ATTACK):
				if (enemySoldier->getState() == "STANDING")
				{
					if (lowestType == "S" && determineDistance(enemySoldier->getXPos(), playerSoldier->getXPos(), enemySoldier->getZPos(), playerSoldier->getZPos()) == 1)
					{
						float rotateAngle = determineDirectionEnemy(playerSoldier->getXPos(), playerSoldier->getZPos(), enemySoldier->getXPos(), enemySoldier->getZPos());
						enemySoldier->setRotateAngle(rotateAngle);
						enemySoldier->setRotatingToAttack();
						name = determineGridName(playerSoldier->getXPos(), playerSoldier->getZPos());
						tile = (GridSurface*)graphics->getGameObject(name);
						int hp = playerSoldier->getHealth();
						hp -= enemySoldier->getPower();
						playerSoldier->setHealth(hp);
						if (playerSoldier->getHealth() <= 0)
						{
							playerSoldier->setHealth(0);
							playerSoldier->setDying();
							playerSoldier->setBehavior(new ShakeBehavior(playerSoldier));
							playerSoldier->setLiving(false);
							tile->setEmpty();
							if (!playerTank->getLiving() && !playerFlyer->getLiving())
								state = PLAYER_LOSE;
						}
					}
					else if (lowestType == "T" && determineDistance(enemySoldier->getXPos(), playerTank->getXPos(), enemySoldier->getZPos(), playerTank->getZPos()) == 1)
					{
						float rotateAngle = determineDirectionEnemy(playerTank->getXPos(), playerTank->getZPos(), enemySoldier->getXPos(), enemySoldier->getZPos());
						enemySoldier->setRotateAngle(rotateAngle);
						enemySoldier->setRotatingToAttack();
						name = determineGridName(playerTank->getXPos(), playerTank->getZPos());
						tile = (GridSurface*)graphics->getGameObject(name);
						int hp = playerTank->getHealth();
						hp -= enemySoldier->getPower();
						playerTank->setHealth(hp);
						if (playerTank->getHealth() <= 0)
						{
							playerTank->setHealth(0);
							playerTank->setDying();
							playerTank->setLiving(false);
							tile->setEmpty();
							if (!playerSoldier->getLiving() && !playerFlyer->getLiving())
								state = PLAYER_LOSE;
						}
					}
					else if (lowestType == "F" && determineDistance(enemySoldier->getXPos(), playerFlyer->getXPos(), enemySoldier->getZPos(), playerFlyer->getZPos()) == 1)
					{
						float rotateAngle = determineDirectionEnemy(playerFlyer->getXPos(), playerFlyer->getZPos(), enemySoldier->getXPos(), enemySoldier->getZPos());
						enemySoldier->setRotateAngle(rotateAngle);
						enemySoldier->setRotatingToAttack();
						name = determineGridName(playerFlyer->getXPos(), playerFlyer->getZPos());
						tile = (GridSurface*)graphics->getGameObject(name);
						int hp = playerFlyer->getHealth();
						hp -= enemySoldier->getPower();
						playerFlyer->setHealth(hp);
						if (playerFlyer->getHealth() <= 0)
						{
							playerFlyer->setHealth(0);
							playerFlyer->setDying();
							playerFlyer->setLiving(false);
							tile->setEmpty();
							if (!playerTank->getLiving() && !playerSoldier->getLiving())
								state = PLAYER_LOSE;
						}
					}

					this->logger->log("Soldier health: " + std::to_string(playerSoldier->getHealth()));
					this->logger->log("Tank health: " + std::to_string(playerTank->getHealth()));
					this->logger->log("Flyer health: " + std::to_string(playerFlyer->getHealth()));

					if (enemyTank->getLiving())
						state = ENEMY_TANK_MOVE;
					else if (enemyFlyer->getLiving())
						state = ENEMY_FLYER_MOVE;
					else
						state = PLAYER_TURN_START;
				}
				break;

			case (ENEMY_TANK_MOVE) :
				//MOVE TANK
				if (playerSoldier->getLiving())
				{
					lowest = determineDistance(enemyTank->getXPos(), playerSoldier->getXPos(), enemyTank->getZPos(), playerSoldier->getZPos());
					lowestType = "S";
				}
				else
					lowest = 999;
				if (playerTank->getLiving())
				{
					check = determineDistance(enemyTank->getXPos(), playerTank->getXPos(), enemyTank->getZPos(), playerTank->getZPos());
					if (check < lowest)
					{
						lowest = check;
						lowestType = "T";
					}//if
					else if (check == lowest)
					{
						if (playerTank->getHealth() < playerSoldier->getHealth())
						{
							lowest = check;
							lowestType = "T";
						}//if
						else
						{
							res = rand() % 2;
							if (res == 0)				//randomly choose, 50% chance
							{
								lowest = check;
								lowestType = "T";
							}//if
						}//else
					}//elseif
				}//if
				if (playerFlyer->getLiving())
				{
					check = determineDistance(enemyTank->getXPos(), playerFlyer->getXPos(), enemyTank->getZPos(), playerFlyer->getZPos());
					if (check < lowest)
					{
						lowest = check;
						lowestType = "F";
					}
					else if (check == lowest)
					{
						if (lowestType == "S")
						{
							if (playerFlyer->getHealth() < playerSoldier->getHealth())
							{
								lowest = check;
								lowestType = "F";
							}
							else
							{
								res = rand() % 2;
								if (res == 0)				//randomly choose, 50% chance
								{
									lowest = check;
									lowestType = "F";
								}
							}
						}
						else if (lowestType == "T")
						{
							if (playerFlyer->getHealth() < playerTank->getHealth())
							{
								lowest = check;
								lowestType = "F";
							}
							else
							{
								res = rand() % 2;
								if (res == 0)				//randomly choose, 50% chance
								{
									lowest = check;
									lowestType = "F";
								}
							}
						}
					}
				}

				if (lowestType == "S")
				{
					targetX = playerSoldier->getXPos();
					targetZ = playerSoldier->getZPos();
				}
				if (lowestType == "T")
				{
					targetX = playerTank->getXPos();
					targetZ = playerTank->getZPos();
				}
				if (lowestType == "F")
				{
					targetX = playerFlyer->getXPos();
					targetZ = playerFlyer->getZPos();
				}

				this->logger->log("Enemy targeted:");
				this->logger->log(lowestType);
				this->logger->log("Distance:");
				this->logger->log(std::to_string(lowest));

				inRange = false;
				if ((lowest - 2) <= 1)
					inRange = true;

				lowest = 999;

				check = determineDistance(enemyTank->getXPos(), targetX, enemyTank->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					lowest = check;
					targetSpaceX = enemyTank->getXPos();
					targetSpaceZ = enemyTank->getZPos();
					OnSameSpace = true;
				}
				check = determineDistance(enemyTank->getXPos() + 2, targetX, enemyTank->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyTank->getXPos() + 2, enemyTank->getZPos());
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyTank->getXPos() + 2;
						targetSpaceZ = enemyTank->getZPos();
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyTank->getXPos() + 1, targetX, enemyTank->getZPos() + 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyTank->getXPos() + 1, enemyTank->getZPos() + 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyTank->getXPos() + 1;
						targetSpaceZ = enemyTank->getZPos() + 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyTank->getXPos() + 1, targetX, enemyTank->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyTank->getXPos() + 1, enemyTank->getZPos());
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyTank->getXPos() + 1;
						targetSpaceZ = enemyTank->getZPos();
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyTank->getXPos() + 1, targetX, enemyTank->getZPos() - 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyTank->getXPos() + 1, enemyTank->getZPos() - 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyTank->getXPos() + 1;
						targetSpaceZ = enemyTank->getZPos() - 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyTank->getXPos(), targetX, enemyTank->getZPos() + 2, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyTank->getXPos(), enemyTank->getZPos() + 2);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyTank->getXPos();
						targetSpaceZ = enemyTank->getZPos() + 2;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyTank->getXPos(), targetX, enemyTank->getZPos() + 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyTank->getXPos(), enemyTank->getZPos() + 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyTank->getXPos();
						targetSpaceZ = enemyTank->getZPos() + 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyTank->getXPos(), targetX, enemyTank->getZPos() - 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyTank->getXPos(), enemyTank->getZPos() - 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyTank->getXPos();
						targetSpaceZ = enemyTank->getZPos() - 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyTank->getXPos(), targetX, enemyTank->getZPos() - 2, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyTank->getXPos(), enemyTank->getZPos() - 2);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyTank->getXPos();
						targetSpaceZ = enemyTank->getZPos() - 2;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyTank->getXPos() - 2, targetX, enemyTank->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyTank->getXPos() - 2, enemyTank->getZPos());
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyTank->getXPos() - 2;
						targetSpaceZ = enemyTank->getZPos();
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyTank->getXPos() - 1, targetX, enemyTank->getZPos() + 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyTank->getXPos() - 1, enemyTank->getZPos() + 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyTank->getXPos() - 1;
						targetSpaceZ = enemyTank->getZPos() + 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyTank->getXPos() - 1, targetX, enemyTank->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyTank->getXPos() - 1, enemyTank->getZPos());
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyTank->getXPos() - 1;
						targetSpaceZ = enemyTank->getZPos();
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyTank->getXPos() - 1, targetX, enemyTank->getZPos() - 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyTank->getXPos() - 1, enemyTank->getZPos() - 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyTank->getXPos() - 1;
						targetSpaceZ = enemyTank->getZPos() - 1;
						OnSameSpace = false;
					}
				}

				name = determineGridName(enemyFlyer->getXPos(), enemyFlyer->getZPos());
				tile = (GridSurface*)graphics->getGameObject(name);
				tile->setEmpty();
				name = determineGridName(targetSpaceX, targetSpaceZ);
				tile = (GridSurface*)graphics->getGameObject(name);
				tile->setEnemy();
				tile->setTank();
				rotateAngle = determineDirectionEnemy(targetSpaceX, targetSpaceZ, enemyTank->getXPos(), enemyTank->getZPos());
				distance = determineMoveScale(targetSpaceX, targetSpaceZ, enemyTank->getXPos(), enemyTank->getZPos());
				enemyTank->setPosXZ(tile->referenceFrame.getPosition().x, tile->referenceFrame.getPosition().z);
				enemyTank->setRotateAngle(rotateAngle);
				enemyTank->setMoveDistance(distance);
				enemyTank->setPosition(targetSpaceX, targetSpaceZ);
				if (!OnSameSpace)
					enemyTank->setRotatingToMove();

				if (inRange)
					state = ENEMY_TANK_ATTACK;
				else if (enemyFlyer->getLiving())
					state = ENEMY_FLYER_MOVE;
				else
					state = PLAYER_TURN_START;

				break;

			case (ENEMY_TANK_ATTACK) :
				if (enemyTank->getState() == "STANDING")
				{
					if (lowestType == "S" && determineDistance(enemyTank->getXPos(), playerSoldier->getXPos(), enemyTank->getZPos(), playerSoldier->getZPos()) == 1)
					{
						float rotateAngle = determineDirectionEnemy(playerSoldier->getXPos(), playerSoldier->getZPos(), enemyTank->getXPos(), enemyTank->getZPos());
						enemyTank->setRotateAngle(rotateAngle);
						enemyTank->setRotatingToAttack();
						name = determineGridName(playerSoldier->getXPos(), playerSoldier->getZPos());
						tile = (GridSurface*)graphics->getGameObject(name);
						int hp = playerSoldier->getHealth();
						hp -= enemyTank->getPower();
						playerSoldier->setHealth(hp);
						if (playerSoldier->getHealth() <= 0)
						{
							playerSoldier->setHealth(0);
							playerSoldier->setLiving(false);
							playerSoldier->setDying();
							playerSoldier->setBehavior(new ShakeBehavior(playerSoldier));
							tile->setEmpty();
							if (!playerTank->getLiving() && !playerFlyer->getLiving())
								state = PLAYER_LOSE;
						}
					}
					else if (lowestType == "T" && determineDistance(enemyTank->getXPos(), playerTank->getXPos(), enemyTank->getZPos(), playerTank->getZPos()) == 1)
					{
						float rotateAngle = determineDirectionEnemy(playerTank->getXPos(), playerTank->getZPos(), enemyTank->getXPos(), enemyTank->getZPos());
						enemyTank->setRotateAngle(rotateAngle);
						enemyTank->setRotatingToAttack();
						name = determineGridName(playerTank->getXPos(), playerTank->getZPos());
						tile = (GridSurface*)graphics->getGameObject(name);
						int hp = playerTank->getHealth();
						hp -= enemyTank->getPower();
						playerTank->setHealth(hp);
						if (playerTank->getHealth() <= 0)
						{
							playerTank->setHealth(0);
							playerTank->setDying();
							playerTank->setLiving(false);
							tile->setEmpty();
							if (!playerSoldier->getLiving() && !playerFlyer->getLiving())
								state = PLAYER_LOSE;
						}
					}
					else if (lowestType == "F" && determineDistance(enemyTank->getXPos(), playerFlyer->getXPos(), enemyTank->getZPos(), playerFlyer->getZPos()) == 1)
					{
						float rotateAngle = determineDirectionEnemy(playerFlyer->getXPos(), playerFlyer->getZPos(), enemyTank->getXPos(), enemyTank->getZPos());
						enemyTank->setRotateAngle(rotateAngle);
						enemyTank->setRotatingToAttack();
						name = determineGridName(playerFlyer->getXPos(), playerFlyer->getZPos());
						tile = (GridSurface*)graphics->getGameObject(name);
						int hp = playerFlyer->getHealth();
						hp -= enemyTank->getPower();
						playerFlyer->setHealth(hp);
						if (playerFlyer->getHealth() <= 0)
						{
							playerFlyer->setHealth(0);
							playerFlyer->setDying();
							playerFlyer->setLiving(false);
							tile->setEmpty();
							if (!playerTank->getLiving() && !playerSoldier->getLiving())
								state = PLAYER_LOSE;
						}
					}

					this->logger->log("Soldier health: " + std::to_string(playerSoldier->getHealth()));
					this->logger->log("Tank health: " + std::to_string(playerTank->getHealth()));
					this->logger->log("Flyer health: " + std::to_string(playerFlyer->getHealth()));

					if (enemyFlyer->getLiving())
						state = ENEMY_FLYER_MOVE;
					else
						state = PLAYER_TURN_START;
				}
				break;

			case (ENEMY_FLYER_MOVE):
				//MOVE FLYER
				if (playerSoldier->getLiving())
				{
					lowest = determineDistance(enemyFlyer->getXPos(), playerSoldier->getXPos(), enemyFlyer->getZPos(), playerSoldier->getZPos());
					lowestType = "S";
				}
				else
					lowest = 999;
				if (playerTank->getLiving())
				{
					check = determineDistance(enemyFlyer->getXPos(), playerTank->getXPos(), enemyFlyer->getZPos(), playerTank->getZPos());
					if (check < lowest)
					{
						lowest = check;
						lowestType = "T";
					}//if
					else if (check == lowest)
					{
						if (playerTank->getHealth() < playerSoldier->getHealth())
						{
							lowest = check;
							lowestType = "T";
						}//if
						else
						{
							res = rand() % 2;
							if (res == 0)				//randomly choose, 50% chance
							{
								lowest = check;
								lowestType = "T";
							}//if
						}//else
					}//elseif
				}//if
				if (playerFlyer->getLiving())
				{
					check = determineDistance(enemyFlyer->getXPos(), playerFlyer->getXPos(), enemyFlyer->getZPos(), playerFlyer->getZPos());
					if (check < lowest)
					{
						lowest = check;
						lowestType = "F";
					}
					else if (check == lowest)
					{
						if (lowestType == "S")
						{
							if (playerFlyer->getHealth() < playerSoldier->getHealth())
							{
								lowest = check;
								lowestType = "F";
							}
							else
							{
								res = rand() % 2;
								if (res == 0)				//randomly choose, 50% chance
								{
									lowest = check;
									lowestType = "F";
								}
							}
						}
						else if (lowestType == "T")
						{
							if (playerFlyer->getHealth() < playerTank->getHealth())
							{
								lowest = check;
								lowestType = "F";
							}
							else
							{
								res = rand() % 2;
								if (res == 0)				//randomly choose, 50% chance
								{
									lowest = check;
									lowestType = "F";
								}
							}
						}
					}
				}

				if (lowestType == "S")
				{
					targetX = playerSoldier->getXPos();
					targetZ = playerSoldier->getZPos();
				}
				if (lowestType == "T")
				{
					targetX = playerTank->getXPos();
					targetZ = playerTank->getZPos();
				}
				if (lowestType == "F")
				{
					targetX = playerFlyer->getXPos();
					targetZ = playerFlyer->getZPos();
				}

				this->logger->log("Enemy targeted:");
				this->logger->log(lowestType);
				this->logger->log("Distance:");
				this->logger->log(std::to_string(lowest));

				inRange = false;
				if ((lowest - 2) <= 1)
					inRange = true;

				lowest = 999;

				check = determineDistance(enemyFlyer->getXPos(), targetX, enemyFlyer->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					lowest = check;
					targetSpaceX = enemyFlyer->getXPos();
					targetSpaceZ = enemyFlyer->getZPos();
					OnSameSpace = true;
				}
				check = determineDistance(enemyFlyer->getXPos() + 2, targetX, enemyFlyer->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyFlyer->getXPos() + 2, enemyFlyer->getZPos());
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyFlyer->getXPos() + 2;
						targetSpaceZ = enemyFlyer->getZPos();
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyFlyer->getXPos() + 1, targetX, enemyFlyer->getZPos() + 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyFlyer->getXPos() + 1, enemyFlyer->getZPos() + 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyFlyer->getXPos() + 1;
						targetSpaceZ = enemyFlyer->getZPos() + 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyFlyer->getXPos() + 1, targetX, enemyFlyer->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyFlyer->getXPos() + 1, enemyFlyer->getZPos());
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyFlyer->getXPos() + 1;
						targetSpaceZ = enemyFlyer->getZPos();
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyFlyer->getXPos() + 1, targetX, enemyFlyer->getZPos() - 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyFlyer->getXPos() + 1, enemyFlyer->getZPos() - 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyFlyer->getXPos() + 1;
						targetSpaceZ = enemyFlyer->getZPos() - 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyFlyer->getXPos(), targetX, enemyFlyer->getZPos() + 2, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyFlyer->getXPos(), enemyFlyer->getZPos() + 2);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyFlyer->getXPos();
						targetSpaceZ = enemyFlyer->getZPos() + 2;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyFlyer->getXPos(), targetX, enemyFlyer->getZPos() + 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyFlyer->getXPos(), enemyFlyer->getZPos() + 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyFlyer->getXPos();
						targetSpaceZ = enemyFlyer->getZPos() + 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyFlyer->getXPos(), targetX, enemyFlyer->getZPos() - 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyFlyer->getXPos(), enemyFlyer->getZPos() - 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyFlyer->getXPos();
						targetSpaceZ = enemyFlyer->getZPos() - 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyFlyer->getXPos(), targetX, enemyFlyer->getZPos() - 2, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyFlyer->getXPos(), enemyFlyer->getZPos() - 2);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyFlyer->getXPos();
						targetSpaceZ = enemyFlyer->getZPos() - 2;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyFlyer->getXPos() - 2, targetX, enemyFlyer->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyFlyer->getXPos() - 2, enemyFlyer->getZPos());
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyFlyer->getXPos() - 2;
						targetSpaceZ = enemyFlyer->getZPos();
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyFlyer->getXPos() - 1, targetX, enemyFlyer->getZPos() + 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyFlyer->getXPos() - 1, enemyFlyer->getZPos() + 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyFlyer->getXPos() - 1;
						targetSpaceZ = enemyFlyer->getZPos() + 1;
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyFlyer->getXPos() - 1, targetX, enemyFlyer->getZPos(), targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyFlyer->getXPos() - 1, enemyFlyer->getZPos());
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyFlyer->getXPos() - 1;
						targetSpaceZ = enemyFlyer->getZPos();
						OnSameSpace = false;
					}
				}
				check = determineDistance(enemyFlyer->getXPos() - 1, targetX, enemyFlyer->getZPos() - 1, targetZ);
				if (check < lowest && check != 0 && lowest != 1)
				{
					name = determineGridName(enemyFlyer->getXPos() - 1, enemyFlyer->getZPos() - 1);
					tile = (GridSurface*)graphics->getGameObject(name);
					if (tile->containsNothing())
					{
						lowest = check;
						targetSpaceX = enemyFlyer->getXPos() - 1;
						targetSpaceZ = enemyFlyer->getZPos() - 1;
						OnSameSpace = false;
					}
				}

				name = determineGridName(enemyFlyer->getXPos(), enemyFlyer->getZPos());
				tile = (GridSurface*)graphics->getGameObject(name);
				tile->setEmpty();
				name = determineGridName(targetSpaceX, targetSpaceZ);
				tile = (GridSurface*)graphics->getGameObject(name);
				tile->setEnemy();
				tile->setFlyer();
				rotateAngle = determineDirectionEnemy(targetSpaceX, targetSpaceZ, enemyFlyer->getXPos(), enemyFlyer->getZPos());
				distance = determineMoveScale(targetSpaceX, targetSpaceZ, enemyFlyer->getXPos(), enemyFlyer->getZPos());
				enemyFlyer->setPosXZ(tile->referenceFrame.getPosition().x, tile->referenceFrame.getPosition().z);
				enemyFlyer->setRotateAngle(rotateAngle);
				enemyFlyer->setMoveDistance(distance);
				enemyFlyer->setPosition(targetSpaceX, targetSpaceZ);
				if (!OnSameSpace)
					enemyFlyer->setRotatingToMove();

				this->logger->log(std::to_string(enemyFlyer->getXPos()) + "X, " + std::to_string(enemyFlyer->getZPos()) + "Z");


				if (inRange)
					state = ENEMY_FLYER_ATTACK;
				else
					state = PLAYER_TURN_START;
				break;

			case(ENEMY_FLYER_ATTACK):
				if (enemyFlyer->getState() == "STANDING")
				{
					if (lowestType == "S" && determineDistance(enemyFlyer->getXPos(), playerSoldier->getXPos(), enemyFlyer->getZPos(), playerSoldier->getZPos()) == 1)
					{
						float rotateAngle = determineDirectionEnemy(playerSoldier->getXPos(), playerSoldier->getZPos(), enemyFlyer->getXPos(), enemyFlyer->getZPos());
						enemyFlyer->setRotateAngle(rotateAngle);
						enemyFlyer->setRotatingToAttack();
						name = determineGridName(playerSoldier->getXPos(), playerSoldier->getZPos());
						tile = (GridSurface*)graphics->getGameObject(name);
						int hp = playerSoldier->getHealth();
						hp -= enemyFlyer->getPower();
						playerSoldier->setHealth(hp);
						if (playerSoldier->getHealth() <= 0)
						{
							playerSoldier->setHealth(0);
							playerSoldier->setLiving(false);
							playerSoldier->setDying();
							playerSoldier->setBehavior(new ShakeBehavior(playerSoldier));
							tile->setEmpty();
							if (!playerTank->getLiving() && !playerFlyer->getLiving())
								state = PLAYER_LOSE;
						}
					}
					else if (lowestType == "T" && determineDistance(enemyFlyer->getXPos(), playerTank->getXPos(), enemyFlyer->getZPos(), playerTank->getZPos()) == 1)
					{
						float rotateAngle = determineDirectionEnemy(playerTank->getXPos(), playerTank->getZPos(), enemyFlyer->getXPos(), enemyFlyer->getZPos());
						enemyFlyer->setRotateAngle(rotateAngle);
						enemyFlyer->setRotatingToAttack();
						name = determineGridName(playerTank->getXPos(), playerTank->getZPos());
						tile = (GridSurface*)graphics->getGameObject(name);
						int hp = playerTank->getHealth();
						hp -= enemyFlyer->getPower();
						playerTank->setHealth(hp);
						if (playerTank->getHealth() <= 0)
						{
							playerTank->setHealth(0);
							playerTank->setDying();
							playerTank->setLiving(false);
							tile->setEmpty();
							if (!playerSoldier->getLiving() && !playerFlyer->getLiving())
								state = PLAYER_LOSE;
						}
					}
					else if (lowestType == "F" && determineDistance(enemyFlyer->getXPos(), playerFlyer->getXPos(), enemyFlyer->getZPos(), playerFlyer->getZPos()) == 1)
					{
						float rotateAngle = determineDirectionEnemy(playerFlyer->getXPos(), playerFlyer->getZPos(), enemyFlyer->getXPos(), enemyFlyer->getZPos());
						enemyFlyer->setRotateAngle(rotateAngle);
						enemyFlyer->setRotatingToAttack();
						name = determineGridName(playerFlyer->getXPos(), playerFlyer->getZPos());
						tile = (GridSurface*)graphics->getGameObject(name);
						int hp = playerFlyer->getHealth();
						hp -= enemyFlyer->getPower();
						playerFlyer->setHealth(hp);
						if (playerFlyer->getHealth() <= 0)
						{
							playerFlyer->setHealth(0);
							playerFlyer->setDying();
							playerFlyer->setLiving(false);
							tile->setEmpty();
							if (!playerTank->getLiving() && !playerSoldier->getLiving())
								state = PLAYER_LOSE;
						}
					}

					this->logger->log("Soldier health: " + std::to_string(playerSoldier->getHealth()));
					this->logger->log("Tank health: " + std::to_string(playerTank->getHealth()));
					this->logger->log("Flyer health: " + std::to_string(playerFlyer->getHealth()));

					state = PLAYER_TURN_START;
				}
				break;

			case (PLAYER_WIN) :
				this->logger->log("Player wins! Are you proud???");

				object = (OGL3DObject*)graphics->getGameObject("WinPlat");
				if (object) {
					object->setVisibility(true);
				}

				break;

			case (PLAYER_LOSE) :
				this->logger->log("Player loses! You suck!");

				object = (OGL3DObject*)graphics->getGameObject("LosePlat");
				if (object) {
					object->setVisibility(true);
				}
				break;
			}
		}
	}
	else
		ZPressed = false;

	if (inputSystem->keys['W']) {
		camera->setMovingForward();
	}
	else if (inputSystem->keys['S']) {
		camera->setMovingBackward();
	}
	if (inputSystem->keys['A']) {
		camera->setMovingLeft();
	}
	else if (inputSystem->keys['D']) {
		camera->setMovingRight();
	}
}

void TheGame::setColor(int x, int z, Texture* t)
{
	if (x > 10 || x < 1 || z > 10 || z < 1)
	{
	}
	else
	{
		string name = determineGridName(x, z);
		
		OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
		TextureManager* texMgr = graphics->getTextureManager();
		OGL3DObject* object = (OGL3DObject*)graphics->getGameObject(name);
		object->setTexture(t);
	}
}

void TheGame::showMoveRange(int x, int z, Texture* t)
{
	setColor(x + 2, z, t);
	setColor(x + 1, z, t);
	setColor(x + 1, z + 1, t);
	setColor(x + 1, z - 1, t);
	setColor(x, z + 2, t);
	setColor(x, z + 1, t);
	setColor(x, z - 1, t);
	setColor(x, z - 2, t);
	setColor(x - 2, z, t);
	setColor(x - 1, z + 1, t);
	setColor(x - 1, z - 1, t);
	setColor(x - 1, z, t);
	currentRange = 2;
}

void TheGame::showAttackRange(int x, int z, Texture* t)
{
	setColor(x + 1, z, t);
	setColor(x - 1, z, t);
	setColor(x, z + 1, t);
	setColor(x, z - 1, t);
	currentRange = 1;
}

int TheGame::determineDistance(int x1, int x2, int z1, int z2)
{
	int x = std::abs(x1 - x2);
	int z = std::abs(z1 - z2);
	return (x + z);
}

string TheGame::determineGridName(int x, int z)
{
	int markNumber = (((z - 1) * 10) + (x - 1)); 
	string name = "Grid";
	std::stringstream ss;
	ss << markNumber;
	name = name + ss.str();
	return name;
}

float TheGame::determineDirection(int cursorX, int cursorZ, int posX, int posZ)
{
	int x = cursorX - posX;
	int z = cursorZ - posZ;
	float rotateAngle;

	//checks our intermediate directions
	if (x < 0 && z < 0)
	{
		direction = BACK_LEFT;
		rotateAngle = -135.0f;
	}
	else if (x > 0 && z > 0)
	{
		direction = FRONT_RIGHT;
		rotateAngle = 45.0f;
	}
	else if (x < 0 && z > 0)
	{
		direction = FRONT_LEFT;
		rotateAngle = -45.0f;
	}
	else if (x > 0 && z < 0)
	{
		direction = BACK_RIGHT;
		rotateAngle = 135.0f;
	}

	//checks our cardinal directions
	else if (x == 0 && z == 0)
	{
		direction = NONE;
		rotateAngle = 0.0f;
	}
	else if (x == 0 && z < 0)
	{
		direction = BACK;
		rotateAngle = 180.0f;
	}
	else if (x == 0 && z > 0)
	{
		direction = FRONT;
		rotateAngle = 0.0f;
	}
	else if (x < 0 && z == 0)
	{
		direction = LEFT;
		rotateAngle = -90.0f;
	}
	else if (x > 0 && z == 0)
	{
		direction = RIGHT;
		rotateAngle = 90.0f;
	}

	return rotateAngle;
}

float TheGame::determineDirectionEnemy(int targetX, int targetZ, int posX, int posZ)
{
	int x = targetX - posX;
	int z = targetZ - posZ;
	float rotateAngle;

	//checks our intermediate directions
	if (x < 0 && z > 0)
	{
		direction = BACK_LEFT;
		rotateAngle = 135.0f;
	}
	else if (x < 0 && z < 0)
	{
		direction = FRONT_RIGHT;
		rotateAngle = 45.0f;
	}
	else if (x > 0 && z < 0)
	{
		direction = FRONT_LEFT;
		rotateAngle = -45.0f;
	}
	else if (x > 0 && z > 0)
	{
		direction = BACK_RIGHT;
		rotateAngle = -135.0f;
	}

	//checks our cardinal directions
	else if (x == 0 && z == 0)
	{
		direction = NONE;
		rotateAngle = 0.0f;
	}
	else if (x == 0 && z > 0)
	{
		direction = BACK;
		rotateAngle = 180.0f;
	}
	else if (x == 0 && z < 0)
	{
		direction = FRONT;
		rotateAngle = 0.0f;
	}
	else if (x > 0 && z == 0)
	{
		direction = LEFT;
		rotateAngle = -90.0f;
	}
	else if (x < 0 && z == 0)
	{
		direction = RIGHT;
		rotateAngle = 90.0f;
	}

	return rotateAngle;
}

float TheGame::determineMoveScale(int cursorX, int cursorZ, int posX, int posZ)
{
	int x = std::abs(cursorX - posX);
	int y = std::abs(cursorZ - posZ);

	if (direction == FRONT_LEFT || direction == FRONT_RIGHT || direction == BACK_LEFT || direction == BACK_RIGHT)
		return (float)std::sqrt(8.0);
	else if (x == 2 || y == 2)
		return 4.0f;
	else if (x == 1 || y == 1)
		return 2.0f;
	else
		return 0.0f;
}