#pragma once
#ifndef TEXTURE_LOADER
#define TEXTURE_LOADER

class Texture;

class ITextureLoader
{
public:
	ITextureLoader();
	virtual ~ITextureLoader();

	virtual Texture* load() = 0;
};

#endif

