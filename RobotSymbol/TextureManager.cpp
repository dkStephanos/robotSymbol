#include "TextureManager.h"
#include "Texture.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	auto it = this->textureMap.begin();
	while (it != this->textureMap.end()) {
		delete it->second;
		it++;
	}
	this->textureMap.clear();
}

void TextureManager::addTexture(const string& textureName, Texture* texture)
{
	if (this->textureMap.find(textureName) == this->textureMap.end()) {
		this->textureMap[textureName] = texture;
	}
}

Texture* TextureManager::getTexture(const string& textureName)
{
	Texture* texture = nullptr;
	if (this->textureMap.find(textureName) != this->textureMap.end()) {
		texture = this->textureMap[textureName];
	}
	return texture;
}
