#pragma once
#ifndef GAME_ASSET_LOADER
#define GAME_ASSET_LOADER

#include "AssetLoader.h"

#include <string>
using std::string;

class GameObjectManager;
class TextureManager;
class Texture;
class OGLTexturedFlatSurface;

class GameAssetLoader : public AssetLoader
{
public:
	GameAssetLoader();

	virtual ~GameAssetLoader();

	void loadAssets();

	void loadOnlyTheShaders();

	void loadOnlyTheUniforms();

protected:
	void loadShaders();

	void loadVertexShaders();

	void loadFragmentShaders();

	void loadShaderPrograms();

	void loadUniforms();

	void loadTextures();

	void loadGameObjects();

	void loadLocalLightSources(char delimeter=',');

	Texture* loadTexture(const string& type, const string& filepath);

	Texture* loadBmpTexture(const string& filepath);

	void loadObject(vector<string>& fields);

	void loadGrid();
	
	void loadAxis();

	void loadOTFS();

	void loadTexturedCuboid();

	void loadSphere();

	void loadCuboid();

	void loadPyramid();

	void loadLineBox();

	void loadConstruct();

	void loadSoldier();

	void loadFlyer();

	void loadTank();

	OGLTexturedFlatSurface* loadSurface();
};

#endif

