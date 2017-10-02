#pragma once
#ifndef BMP_TEXTURE_LOADER
#define BMP_TEXTURE_LOADER

#include "ITextureLoader.h"

#include <string>
using std::string;

class Texture;

class BMPTextureLoader :
	public ITextureLoader
{
protected:
	string filepath;

private:
	string status;

public:
	BMPTextureLoader();
	virtual ~BMPTextureLoader();

	void setFilePath(const string& filepath) { this->filepath = filepath; }

	const string& getFileStatus() { return this->status; }

	Texture* load();
};

#endif

