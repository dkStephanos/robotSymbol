#include "GameAssetLoader.h"
#include "TextureManager.h"
#include "BMPTextureLoader.h"
#include "OGL2DTexture.h"
#include "OGLObject.h"
#include "GameObjectManager.h"
#include "Axis.h"
#include "OGLTexturedFlatSurface.h"
#include "Sphere.h"
#include "Cuboid.h"
#include "OGLShaderManager.h"
#include "GameEngine.h"
#include "OGLGraphicsSystem.h"
#include "GameWorld.h"
#include "OGLVertexShader.h"
#include "TextFileReader.h"
#include "CoreSystem.h"
#include "OGLFragmentShader.h"
#include "Uniform.h"
#include "LightSource.h"
#include "TexturedCuboid.h"
#include "LineBox.h"
#include "Construct.h"
#include "Logger.h"
#include "Pyramid.h"
#include "GridSurface.h"
#include "Soldier.h"
#include "Flyer.h"
#include <string>
#include <iostream>
#include <sstream>
#include "Tank.h"

GameAssetLoader::GameAssetLoader()
{
}

GameAssetLoader::~GameAssetLoader()
{
}

void GameAssetLoader::loadAssets()
{
	GameObjectManager* gameObjectManager = 
		this->gameEngine->getGraphicsSystem()->getGameWorld()->getObjectManager();
	this->setAtStart();
	if (this->currLine() != "<assets>") return;
	while (!this->atEnd() && this->nextLine() != "</assets>") {
		if (this->currLine() == "<textures>") {
			this->loadTextures();
		}
		else if (this->currLine() == "<gameobjects>") {
			this->loadGameObjects();
		}
	}
}

void GameAssetLoader::loadOnlyTheShaders()
{
	this->setAtStart();
	if (this->currLine() != "<assets>") return;
	while (!this->atEnd() && this->nextLine() != "</assets>") {
		if (this->currLine() == "<shaders>") {
			this->loadShaders();
			break;
		}
	}
}

void GameAssetLoader::loadOnlyTheUniforms()
{
	OGLGraphicsSystem* graphics =
		(OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader;
	shader = (OGLShaderProgram*)shaderMgr->getShader("SimpleShader");
	this->setAtStart();
	if (this->currLine() != "<assets>") return;
	while (!this->atEnd() && this->nextLine() != "</assets>") {
		if (this->currLine() == "<uniforms>") {
			this->loadUniforms();
			break;
		}
	}
}

void GameAssetLoader::loadShaders()
{
	while (!this->atEnd() && this->nextLine() != "</shaders>") {
		if (this->currLine() == "<vertexShaders>") {
			this->loadVertexShaders();
		}
		else if (this->currLine() == "<fragmentShaders>") {
			this->loadFragmentShaders();
		}
		else if (this->currLine() == "<shaderPrograms>") {
			this->loadShaderPrograms();
		}
	}
}

void GameAssetLoader::loadVertexShaders()
{
	OGLGraphicsSystem* graphics = 
		(OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	CoreSystem* core = this->gameEngine->getCoreSystem();
	TextFileReader *reader = core->getTextFileReader();

	string shaderName;
	string fileName;
	vector<string> fields;
	while (!this->atEnd() && this->nextLine() != "</vertexShaders>") {
		this->split(fields, this->currLine());
		shaderName = fields[0];
		switch (fields.size()) {
		case 1:
			graphics->addVertexShader(
				shaderName,
				new OGLVertexShader()
			);
			break;
		case 2:
			fileName = fields[1];
			graphics->addVertexShader(
				shaderName,
				new OGLVertexShader(),
				reader->readContents(fileName)
			);
			break;
		}
		fields.clear();
	}
}

void GameAssetLoader::loadFragmentShaders()
{
	OGLGraphicsSystem* graphics =
		(OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	CoreSystem* core = this->gameEngine->getCoreSystem();
	TextFileReader *reader = core->getTextFileReader();

	string shaderName;
	string fileName;
	vector<string> fields;
	while (!this->atEnd() && this->nextLine() != "</fragmentShaders>") {
		this->split(fields, this->currLine());
		shaderName = fields[0];
		switch (fields.size()) {
		case 1:
			graphics->addFragmentShader(
				shaderName,
				new OGLFragmentShader()
				);
			break;
		case 2:
			fileName = fields[1];
			graphics->addFragmentShader(
				shaderName,
				new OGLFragmentShader(),
				reader->readContents(fileName)
				);
			break;
		}
		fields.clear();
	}
}

void GameAssetLoader::loadShaderPrograms()
{
	OGLGraphicsSystem* graphics =
		(OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();

	string shaderName, vertexName, fragmentName;
	vector<string> fields;
	while (!this->atEnd() && this->nextLine() != "</shaderPrograms>") {
		this->split(fields, this->currLine());
		shaderName = fields[0];
		vertexName = fields[1];
		fragmentName = fields[2];
		graphics->addShaderProgram(
			shaderName,
			new OGLShaderProgram(),
			vertexName, fragmentName
			);
		fields.clear();
	}
}

void GameAssetLoader::loadUniforms()
{
	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader;

	string shaderName, uniformName, uniformTypeName;
	vector<string> fields;
	while (!this->atEnd() && this->nextLine() != "</uniforms>") {
		this->split(fields, this->currLine());
		shaderName = fields[0];
		uniformName = fields[1];
		uniformTypeName = fields[2];
		shader = (OGLShaderProgram*)shaderMgr->getShader(shaderName);
		shader->addUniform(uniformName, Uniform::getType(uniformTypeName));
		fields.clear();
	}
}

void GameAssetLoader::loadGameObjects()
{
	vector<string> fields;
	while (!this->atEnd() && this->nextLine() != "</gameobjects>") {
		if (this->currLine() == "<axis>") {
			this->loadAxis();
			this->logger->log("Axis loaded");
		}
		else if (this->currLine() == "<otfs>") {
			this->loadOTFS();
			this->logger->log("OTFS loaded");
		}
		else if (this->currLine() == "<localLightSources>") {
			this->loadLocalLightSources();
			this->logger->log("Light sources loaded");
		}
		else if (this->currLine() == "<texturedCuboid>") {
			this->loadTexturedCuboid();
			this->logger->log("Textured cuboid loaded");
		}
		else if (this->currLine() == "<sphere>") {
			this->loadSphere();
			this->logger->log("Sphere loaded");
		}
		else if (this->currLine() == "<cuboid>") {
			this->loadCuboid();
			this->logger->log("Cuboid loaded");
		}
		else if (this->currLine() == "<pyramid>") {
			this->loadPyramid();
			this->logger->log("Pyramid loaded");
		}
		else if (this->currLine() == "<linebox>") {
			this->loadLineBox();
			this->logger->log("Linebox loaded");
		}
		else if (this->currLine() == "<construct>") {
			this->loadConstruct();
			this->logger->log("Constructs loaded");
		}
		else if (this->currLine() == "<soldier>") {
			this->loadSoldier();
			this->logger->log("Soldier loaded");
		}
		else if (this->currLine() == "<flyer>") {
			this->loadFlyer();
			this->logger->log("Flyer loaded");
		}
		else if (this->currLine() == "<tank>") {
			this->loadTank();
			this->logger->log("Tank loaded");
		}
		else if (this->currLine() == "<grid>") {
			this->loadGrid();
			this->logger->log("Grid loaded");
		}
	}
}

void GameAssetLoader::loadGrid()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float width = 5, depth = 5;
	unsigned int widthSegments = 5, depthSegments = 5;
	RGBA color = { 1, 1, 1, 1 };
	string textureName, shaderName;
	bool hasBoundingBox = false;
	float bbWidth, bbDepth, bbHeight;

	while (!this->atEnd() && this->nextLine() != "</grid>") {
		if (this->currLine() == "<name>") {
			this->loadElement(fields, "</name>");
			name = fields[0];
		}
		else if (this->currLine() == "<width>") {
			this->loadElement(fields, "</width>");
			width = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<depth>") {
			this->loadElement(fields, "</depth>");
			depth = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<widthSegments>") {
			this->loadElement(fields, "</widthSegments>");
			widthSegments = std::stoi(fields[0], &sz);
		}
		else if (this->currLine() == "<depthSegments>") {
			this->loadElement(fields, "</depthSegments>");
			depthSegments = std::stoi(fields[0], &sz);
		}
		else if (this->currLine() == "<color>") {
			this->loadElement(fields, "</color>");
			color = {
				std::stof(fields[0], &sz),
				std::stof(fields[1], &sz),
				std::stof(fields[2], &sz),
				std::stof(fields[3], &sz)
			};
		}
		else if (this->currLine() == "<boundingbox>") {
			this->loadElement(fields, "</boundingbox>");
			bbWidth = std::stof(fields[0], &sz);
			bbDepth = std::stof(fields[1], &sz);
			bbHeight = std::stof(fields[2], &sz);
			hasBoundingBox = true;
		}
		else if (this->currLine() == "<textureName>") {
			this->loadElement(fields, "</textureName>");
			textureName = fields[0];
		}
		else if (this->currLine() == "<shaderName>") {
			this->loadElement(fields, "</shaderName>");
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader(shaderName);
	Texture* texture = textureMgr->getTexture(textureName);

	for (int x = 0; x < 100; x++)
	{
		name = "Grid";
		std::stringstream ss;
		ss << x;
		name = name + ss.str();
		object = new GridSurface(name, width, depth, widthSegments, depthSegments, color);
		if (hasBoundingBox) {
			object->boundingBox.set(bbWidth, bbDepth, bbHeight);
			object->boundingBox.use = true;
		}
		object->setTexture(texture);
		object->setShaderProgram(shader->getHandle());
		objectMgr->addObject(name, object);
	}
}


void GameAssetLoader::loadAxis()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float length = 1;
	string shaderName;

	while (!this->atEnd() && this->nextLine() != "</axis>") {
		if (this->currLine() == "<name>") {
			this->loadElement(fields, "</name>");
			name = fields[0];
		}
		else if (this->currLine() == "<length>") {
			this->loadElement(fields, "</length>");
			length = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<shaderName>") {
			this->loadElement(fields, "</shaderName>");
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader(shaderName);

	object = new Axis(name, length);
	object->setShaderProgram(shader->getHandle());
	objectMgr->addObject(name, object);
}

void GameAssetLoader::loadOTFS()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float width = 5, depth = 5;
	unsigned int widthSegments = 5, depthSegments = 5;
	RGBA color = { 1, 1, 1, 1 };
	string textureName, shaderName;
	bool hasBoundingBox = false;
	float bbWidth, bbDepth, bbHeight;
	
	while (!this->atEnd() && this->nextLine() != "</otfs>") {
		if (this->currLine() == "<name>") {
			this->loadElement(fields, "</name>");
			name = fields[0];
		}
		else if (this->currLine() == "<width>") {
			this->loadElement(fields, "</width>");
			width = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<depth>") {
			this->loadElement(fields, "</depth>");
			depth = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<widthSegments>") {
			this->loadElement(fields, "</widthSegments>");
			widthSegments = std::stoi(fields[0], &sz);
		}
		else if (this->currLine() == "<depthSegments>") {
			this->loadElement(fields, "</depthSegments>");
			depthSegments = std::stoi(fields[0], &sz);
		}
		else if (this->currLine() == "<color>") {
			this->loadElement(fields, "</color>");
			color = {
				std::stof(fields[0], &sz),
				std::stof(fields[1], &sz),
				std::stof(fields[2], &sz),
				std::stof(fields[3], &sz)
			};
		}
		else if (this->currLine() == "<boundingbox>") {
			this->loadElement(fields, "</boundingbox>");
			bbWidth = std::stof(fields[0], &sz);
			bbDepth = std::stof(fields[1], &sz);
			bbHeight = std::stof(fields[2], &sz);
			hasBoundingBox = true;
		}
		else if (this->currLine() == "<textureName>") {
			this->loadElement(fields, "</textureName>");
			textureName = fields[0];
		}
		else if (this->currLine() == "<shaderName>") {
			this->loadElement(fields, "</shaderName>");
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader(shaderName);
	Texture* texture = textureMgr->getTexture(textureName);

	object = new OGLTexturedFlatSurface(name, width, depth, widthSegments, depthSegments, color);
	if (hasBoundingBox) {
		object->boundingBox.set(bbWidth, bbDepth, bbHeight);
		object->boundingBox.use = true;
	}
	object->setTexture(texture);
	object->setShaderProgram(shader->getHandle());
	objectMgr->addObject(name, object);
}

void GameAssetLoader::loadSoldier()
{
	string name, shaderName, darkMetalTextureName, lightMetalTextureName, goldTextureName, gunTextureName, laserTextureName;
	Soldier *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	while (!this->atEnd() && this->nextLine() != "</soldier>") {
		if (this->currLine() == "<name>") {
			this->loadElement(fields, "</name>");
			name = fields[0];
		}
		else if (this->currLine() == "<darkMetalTexture>")
		{
			this->loadElement(fields, "</darkMetalTexture>");
			darkMetalTextureName = fields[0];
		}
		else if (this->currLine() == "<lightMetalTexture>")
		{
			this->loadElement(fields, "</lightMetalTexture>");
			lightMetalTextureName = fields[0];
		}
		else if (this->currLine() == "<goldTexture>")
		{
			this->loadElement(fields, "</goldTexture>");
			goldTextureName = fields[0];
		}
		else if (this->currLine() == "<laserTexture>")
		{
			this->loadElement(fields, "</laserTexture>");
			laserTextureName = fields[0];
		}
		else if (this->currLine() == "<shaderName>") {
			this->loadElement(fields, "</shaderName>");
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader(shaderName);
	Texture* darkMetalTexture = textureMgr->getTexture(darkMetalTextureName);
	Texture* lightMetalTexture = textureMgr->getTexture(lightMetalTextureName);
	Texture* gunTexture = textureMgr->getTexture(gunTextureName);
	Texture* goldTexture = textureMgr->getTexture(goldTextureName);
	Texture* laserTexture = textureMgr->getTexture(laserTextureName);

	object = new Soldier(name);
	object->setHeadTexture(darkMetalTexture);
	object->setTorsoTexture(darkMetalTexture);
	object->setFootTexture(darkMetalTexture);
	object->setGunTexture(gunTexture);
	object->setShoulderTexture(goldTexture);
	object->setArmTexture(lightMetalTexture);
	object->setLegTexture(lightMetalTexture);
	object->setLaserTexture(laserTexture);
	object->setShaderProgram(shader->getHandle());
	objectMgr->addObject(name, object);

}

void GameAssetLoader::loadTank()
{
	string name, shaderName, textureName, lazerTextureName;
	Tank *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	while (!this->atEnd() && this->nextLine() != "</tank>") {
		if (this->currLine() == "<name>") {
			this->loadElement(fields, "</name>");
			name = fields[0];
		}
		else if (this->currLine() == "<shaderName>") {
			this->loadElement(fields, "</shaderName>");
			shaderName = fields[0];
		}
		else if (this->currLine() == "<textureName>")
		{
			this->loadElement(fields, "</textureName>");
			textureName = fields[0];
		}
		else if (this->currLine() == "<lazerTextureName>")
		{
			this->loadElement(fields, "</lazerTextureName>");
			lazerTextureName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader(shaderName);
	Texture* robotTexture = textureMgr->getTexture(textureName);
	Texture* lazerTexture = textureMgr->getTexture(lazerTextureName);

	object = new Tank(name);
	object->setShaderProgram(shader->getHandle());
	objectMgr->addObject(name, object);
	object->setTexture(robotTexture);
	object->setLazerTexture(lazerTexture);
}

void GameAssetLoader::loadFlyer()
{
	string name, shaderName, robotTextureName, faceTextureName, laserTextureName;
	Flyer *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	while (!this->atEnd() && this->nextLine() != "</flyer>") {
		if (this->currLine() == "<name>") {
			this->loadElement(fields, "</name>");
			name = fields[0];
		}
		else if (this->currLine() == "<robotTexture>")
		{
			this->loadElement(fields, "</robotTexture>");
			robotTextureName = fields[0];
		}
		else if (this->currLine() == "<faceTexture>")
		{
			this->loadElement(fields, "</faceTexture>");
			faceTextureName = fields[0];
		}
		else if (this->currLine() == "<laserTexture>")
		{
			this->loadElement(fields, "</laserTexture>");
			laserTextureName = fields[0];
		}
		else if (this->currLine() == "<shaderName>") {
			this->loadElement(fields, "</shaderName>");
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader(shaderName);
	Texture* robotTexture = textureMgr->getTexture(robotTextureName);
	Texture* faceTexture = textureMgr->getTexture(faceTextureName);
	Texture* laserTexture = textureMgr->getTexture(laserTextureName);

	object = new Flyer(name);
	object->setRobotTexture(robotTexture);
	object->setFaceTexture(faceTexture);
	object->setLaserTexture(laserTexture);
	object->setShaderProgram(shader->getHandle());
	objectMgr->addObject(name, object);

}

void GameAssetLoader::loadTexturedCuboid()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float width = 1, depth = 1, height = 1;
	RGBA color = { 1, 1, 1, 1 };
	string textureName, shaderName;
	bool hasBoundingBox = false;
	float bbWidth, bbDepth, bbHeight;

	while (!this->atEnd() && this->nextLine() != "</texturedCuboid>") {
		if (this->currLine() == "<name>") {
			this->loadElement(fields, "</name>");
			name = fields[0];
		}
		else if (this->currLine() == "<width>") {
			this->loadElement(fields, "</width>");
			width = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<depth>") {
			this->loadElement(fields, "</depth>");
			depth = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<height>") {
			this->loadElement(fields, "</height>");
			height = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<color>") {
			this->loadElement(fields, "</color>");
			color = {
				std::stof(fields[0], &sz),
				std::stof(fields[1], &sz),
				std::stof(fields[2], &sz),
				std::stof(fields[3], &sz)
			};
		}
		else if (this->currLine() == "<boundingbox>") {
			this->loadElement(fields, "</boundingbox>");
			bbWidth = std::stof(fields[0], &sz);
			bbDepth = std::stof(fields[1], &sz);
			bbHeight = std::stof(fields[2], &sz);
			hasBoundingBox = true;
		}
		else if (this->currLine() == "<textureName>") {
			this->loadElement(fields, "</textureName>");
			textureName = fields[0];
		}
		else if (this->currLine() == "<shaderName>") {
			this->loadElement(fields, "</shaderName>");
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader(shaderName);
	Texture* texture = textureMgr->getTexture(textureName);

	object = new TexturedCuboid(name, width, depth, height, color);
	if (hasBoundingBox) {
		object->boundingBox.set(bbWidth, bbDepth, bbHeight);
		object->boundingBox.use = true;
	}
	object->setTexture(texture);
	object->setShaderProgram(shader->getHandle());
	objectMgr->addObject(name, object);
}

void GameAssetLoader::loadCuboid()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float width = 1, depth = 1, height = 1;
	RGBA color = { 1, 1, 1, 1 };
	string shaderName;
	bool hasBoundingBox = false;
	float bbWidth, bbDepth, bbHeight;

	while (!this->atEnd() && this->nextLine() != "</cuboid>") {
		if (this->currLine() == "<name>") {
			this->loadElement(fields, "</name>");
			name = fields[0];
		}
		else if (this->currLine() == "<width>") {
			this->loadElement(fields, "</width>");
			width = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<depth>") {
			this->loadElement(fields, "</depth>");
			depth = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<height>") {
			this->loadElement(fields, "</height>");
			height = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<color>") {
			this->loadElement(fields, "</color>");
			color = {
				std::stof(fields[0], &sz),
				std::stof(fields[1], &sz),
				std::stof(fields[2], &sz),
				std::stof(fields[3], &sz)
			};
		}
		else if (this->currLine() == "<boundingbox>") {
			this->loadElement(fields, "</boundingbox>");
			bbWidth = std::stof(fields[0], &sz);
			bbDepth = std::stof(fields[1], &sz);
			bbHeight = std::stof(fields[2], &sz);
			hasBoundingBox = true;
		}
		else if (this->currLine() == "<shaderName>") {
			this->loadElement(fields, "</shaderName>");
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader(shaderName);

	object = new Cuboid(name, width, depth, height, color);
	if (hasBoundingBox) {
		object->boundingBox.set(bbWidth, bbDepth, bbHeight);
		object->boundingBox.use = true;
	}
	object->setShaderProgram(shader->getHandle());
	objectMgr->addObject(name, object);
}

void GameAssetLoader::loadPyramid()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float width = 1, depth = 1, height = 1;
	RGBA color = { 1, 1, 1, 1 };
	string textureName, shaderName;
	bool hasBoundingBox = false;
	float bbWidth, bbDepth, bbHeight;

	while (!this->atEnd() && this->nextLine() != "</pyramid>") {
		if (this->currLine() == "<name>") {
			this->loadElement(fields, "</name>");
			name = fields[0];
		}
		else if (this->currLine() == "<width>") {
			this->loadElement(fields, "</width>");
			width = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<depth>") {
			this->loadElement(fields, "</depth>");
			depth = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<height>") {
			this->loadElement(fields, "</height>");
			height = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<textureName>") {
			this->loadElement(fields, "</textureName>");
			textureName = fields[0];
		}
		else if (this->currLine() == "<color>") {
			this->loadElement(fields, "</color>");
			color = {
				std::stof(fields[0], &sz),
				std::stof(fields[1], &sz),
				std::stof(fields[2], &sz),
				std::stof(fields[3], &sz)
			};
		}
		else if (this->currLine() == "<boundingbox>") {
			this->loadElement(fields, "</boundingbox>");
			bbWidth = std::stof(fields[0], &sz);
			bbDepth = std::stof(fields[1], &sz);
			bbHeight = std::stof(fields[2], &sz);
			hasBoundingBox = true;
		}
		else if (this->currLine() == "<shaderName>") {
			this->loadElement(fields, "</shaderName>");
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader(shaderName);
	Texture* texture = textureMgr->getTexture(textureName);
	
	object = new Pyramid(name, width, depth, height, color);
	if (hasBoundingBox) {
		object->boundingBox.set(bbWidth, bbDepth, bbHeight);
		object->boundingBox.use = true;
	}
	object->setTexture(texture);
	object->setShaderProgram(shader->getHandle());
	objectMgr->addObject(name, object);
}

void GameAssetLoader::loadLineBox()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float width = 1, depth = 1, height = 1;
	RGBA color = { 1, 1, 1, 1 };
	string shaderName;

	while (!this->atEnd() && this->nextLine() != "</linebox>") {
		if (this->currLine() == "<name>") {
			this->loadElement(fields, "</name>");
			name = fields[0];
		}
		else if (this->currLine() == "<width>") {
			this->loadElement(fields, "</width>");
			width = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<depth>") {
			this->loadElement(fields, "</depth>");
			depth = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<height>") {
			this->loadElement(fields, "</height>");
			height = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<color>") {
			this->loadElement(fields, "</color>");
			color = {
				std::stof(fields[0], &sz),
				std::stof(fields[1], &sz),
				std::stof(fields[2], &sz),
				std::stof(fields[3], &sz)
			};
		}
		else if (this->currLine() == "<shaderName>") {
			this->loadElement(fields, "</shaderName>");
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader(shaderName);

	object = new LineBox(name, width, depth, height, color);
	object->setShaderProgram(shader->getHandle());
	objectMgr->addObject(name, object);
}

void GameAssetLoader::loadConstruct()
{
	Construct *construct = nullptr;
	vector<string> fields;

	OGLTexturedFlatSurface* surface;
	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	string name;

	while (!this->atEnd() && this->nextLine() != "</construct>") {
		if (this->currLine() == "<name>") {
			this->loadElement(fields, "</name>");
			name = fields[0];
			construct = new Construct(name);
		}
		else if (this->currLine() == "<position>") {
			this->loadElement(fields, "</position>");
			construct->referenceFrame.translateWorld(
				std::stof(fields[0]),
				std::stof(fields[1]),
				std::stof(fields[2]));
		}
		else if (this->currLine() == "<visible>") {
			this->loadElement(fields, "</visible>");
			if (fields[0] == "true") {
				construct->setVisibility(true);
			}
			else {
				construct->setVisibility(false);
			}
		}
		else if (this->currLine() == "<surface>") {
			surface = this->loadSurface();
			construct->surfaces[surface->getName()] = surface;
		}
		fields.clear();
	}
	
	objectMgr->addObject(construct->getName(), construct);
}

OGLTexturedFlatSurface* GameAssetLoader::loadSurface()
{
	OGLTexturedFlatSurface *surface = nullptr;
	vector<string> fields;
	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	string name;
	Transform T;

	while (!this->atEnd() && this->nextLine() != "</surface>") {
		if (this->currLine() == "<name>") {
			this->loadElement(fields, "</name>");
			name = fields[0];
			surface = (OGLTexturedFlatSurface*)graphics->getGameObject(name);
		}
		else if (this->currLine() == "<transforms>") {
			this->loadElement(fields, "</transforms>");
			for (size_t i = 0; i < fields.size(); i+=4) {
				T = {
					fields[i],
					std::stof(fields[i+1]),
					std::stof(fields[i+2]),
					std::stof(fields[i+3])
				};
				surface->referenceFrame.transforms.push_back(T);
			}
		}
		fields.clear();
	}
	return surface;
}

void GameAssetLoader::loadSphere()
{
	OGLObject *object = nullptr;
	std::string::size_type sz;
	vector<string> fields;

	string name;
	float radius = 1;
	int slices = 10, stacks = 10;
	RGBA color = { 1, 1, 1, 1 };
	string shaderName;
	bool hasBoundingBox = false;
	float bbWidth, bbDepth, bbHeight;

	while (!this->atEnd() && this->nextLine() != "</sphere>") {
		if (this->currLine() == "<name>") {
			this->loadElement(fields, "</name>");
			name = fields[0];
		}
		else if (this->currLine() == "<radius>") {
			this->loadElement(fields, "</radius>");
			radius = std::stof(fields[0], &sz);
		}
		else if (this->currLine() == "<slices>") {
			this->loadElement(fields, "</slices>");
			slices = std::stoi(fields[0], &sz);
		}
		else if (this->currLine() == "<stacks>") {
			this->loadElement(fields, "</stacks>");
			stacks = std::stoi(fields[0], &sz);
		}
		else if (this->currLine() == "<color>") {
			this->loadElement(fields, "</color>");
			color = {
				std::stof(fields[0], &sz),
				std::stof(fields[1], &sz),
				std::stof(fields[2], &sz),
				std::stof(fields[3], &sz)
			};
		}
		else if (this->currLine() == "<boundingbox>") {
			this->loadElement(fields, "</boundingbox>");
			bbWidth = std::stof(fields[0], &sz);
			bbDepth = std::stof(fields[1], &sz);
			bbHeight = std::stof(fields[2], &sz);
			hasBoundingBox = true;
		}
		else if (this->currLine() == "<shaderName>") {
			this->loadElement(fields, "</shaderName>");
			shaderName = fields[0];
		}
		fields.clear();
	}

	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	GameObjectManager* objectMgr = graphics->getGameWorld()->getObjectManager();
	TextureManager* textureMgr = (TextureManager*)graphics->getTextureManager();
	OGLShaderManager* shaderMgr = graphics->getOGLShaderManager();
	OGLShaderProgram *shader = (OGLShaderProgram*)shaderMgr->getShader(shaderName);

	object = new Sphere(name, radius, slices, stacks, color);
	if (hasBoundingBox) {
		object->boundingBox.set(bbWidth, bbDepth, bbHeight);
		object->boundingBox.use = true;
	}
	object->setShaderProgram(shader->getHandle());
	objectMgr->addObject(name, object);
}

void GameAssetLoader::loadLocalLightSources(char delimeter)
{
	LightSource* light;
	OGLGraphicsSystem* graphics = (OGLGraphicsSystem*)this->gameEngine->getGraphicsSystem();
	std::string::size_type sz;
	float x, y, z, intensity;
	vector<string> fields;
	while (!this->atEnd() && this->nextLine() != "</localLightSources>") {
		this->split(fields, this->currLine(), delimeter);
		x = std::stof(fields[0], &sz);
		y = std::stof(fields[1], &sz);
		z = std::stof(fields[2], &sz);
		intensity = std::stof(fields[3], &sz);
		light = new LightSource();
		light->setPosition(x, y, z);
		light->setIntensity(intensity);
		graphics->getGameWorld()->localLights.push_back(light);
		fields.clear();
	}
}

void GameAssetLoader::loadObject(vector<string>& fields)
{
	Texture* texture = nullptr;
	OGLObject *object = nullptr;
	std::string::size_type sz;
	string name;
	if (fields[0] == "<axis>") {
		name = fields[1];
		float length = std::stof(fields[2], &sz);
		object = new Axis(name, length);
	}

	if (object) {
		GameObjectManager* objectMgr =
			this->gameEngine->getGraphicsSystem()->getGameWorld()->getObjectManager();
		objectMgr->addObject(name, object);
	}
}

void GameAssetLoader::loadTextures()
{
	string type, filepath, textureName;
	Texture* texture;
	vector<string> fields;
	while (!this->atEnd() && this->nextLine() != "</textures>") {
		fields.clear();
		this->split(fields, this->currLine(), ',');
		type = fields[0];
		filepath = fields[1];
		textureName = fields[2];
		texture = this->loadTexture(type, filepath);
		if (texture) {
			TextureManager* textureMgr =
				(TextureManager*)this->gameEngine->getGraphicsSystem()->getTextureManager();
			textureMgr->addTexture(textureName, texture);
		}
	}
}

Texture* GameAssetLoader::loadTexture(const string& type, const string& filepath)
{
	Texture* texture = nullptr;
	if (type == "BMP") {
		texture = this->loadBmpTexture(filepath);
	}
	return texture;
}

Texture* GameAssetLoader::loadBmpTexture(const string& filepath)
{
	BMPTextureLoader bmpLoader;
	bmpLoader.setFilePath(filepath);
	OGL2DTexture* texture = new OGL2DTexture(bmpLoader.load());
	texture->setTexelFormat(GL_BGR);
	texture->setTypeOfData(GL_UNSIGNED_BYTE);
	texture->create();
	return texture;
}


