#pragma once
#ifndef GRID_SURFACE
#define GRID_SURFACE

#include "OGL3DObject.h"

#include <string>
using std::string;

class GridSurface :
	public OGL3DObject
{
protected:
	float width, depth;
	unsigned int numberOfWidthSegments, numberOfDepthSegments;
	RGBA surfaceColor;
	enum Contains { EMPTY, PLAYER, ENEMY };
	Contains contains = EMPTY;
	enum ContainsType { NA, SOLDIER, TANK, FLYER };
	ContainsType containsType = NA;

public:
	GridSurface(
		const string& name, 
		float width=5.0f, float depth=5.0f, 
		unsigned int numberOfWidthSegments=5, unsigned int numberOfDepthSegments=5, 
		RGBA surfaceColor = { 1, 1, 1, 1 });

	virtual ~GridSurface();

	float getWidth() { return this->width; }

	float getDepth() { return this->depth; }

	bool containsEnemy();
	bool containsPlayer();
	bool containsNothing();

	void setEnemy() {this->contains = ENEMY; }
	void setPlayer() { this->contains = PLAYER; }
	void setEmpty() { this->contains = EMPTY; }

	void setSoldier() { this->containsType = SOLDIER; }
	void setTank() { this->containsType = TANK; }
	void setFlyer() { this->containsType = FLYER; }

	string getContainsType();

	void setColor(int x, int z, Texture* t); 

protected:

	void generate();
};

#endif

