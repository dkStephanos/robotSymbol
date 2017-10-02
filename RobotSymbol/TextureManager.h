#pragma once
#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER

#include <map>
using std::map;
#include <string>
using std::string;

class Texture;
// Manages all the textures of the game engine
//
class TextureManager
{
protected:
	map<string, Texture*> textureMap;

public:
	TextureManager();
	virtual ~TextureManager();

	void addTexture(const string& textureName, Texture* texture);

	Texture* getTexture(const string& textureName);
};

#endif

