#pragma once
#ifndef ASSET_LOADER
#define ASSET_LOADER

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "BaseObject.h"

class GameEngine;

class AssetLoader : public BaseObject
{
private:
	vector<string> lines;
	size_t ci;

protected:
	GameEngine* gameEngine;

public:
	AssetLoader();

	virtual ~AssetLoader();

	virtual void setGameEngine(GameEngine* gameEngine) {
		this->gameEngine = gameEngine;
	}

	virtual void loadAssetFile(const string& filepath);

	virtual void loadAssets() = 0;

	virtual void setAtStart() { this->ci = 0; }

	virtual bool isFileLoaded() { return this->lines.size() > 0; }

protected:
	string nextLine();

	string currLine() { return this->lines[this->ci]; }

	bool atEnd() { return this->ci == this->lines.size() - 1; }

	void split(vector<string>& fields, const string& record, char delimeter=',');

	void trim(string& str);

	void loadElement(vector<string>& fields, const string& end, char delimeter=',');
};

#endif

