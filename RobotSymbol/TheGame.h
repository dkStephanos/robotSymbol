#pragma once
#ifndef THE_GAME
#define THE_GAME

#include "BaseObject.h"
#include "Texture.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

class GameEngine;
class AssetLoader;

class TheGame : public BaseObject
{
public:
	GameEngine *gameEngine;
	string lowestType;
	int res;
	bool OnSameSpace, inRange;
	int cursorX, cursorZ, pressedPosX, pressedPosZ, currentRange;
	bool showingRange = false, cursorOn = false, showingAttackRange = false;
	bool TPressed = false, 
		GPressed = false, 
		FPressed = false, 
		HPressed = false, 
		PPressed = false, 
		OPressed = false, 
		ZPressed = false;

	enum State {
		PLAYER_TURN_START,
		PLAYER_SOLDIER_MOVE,
		PLAYER_SOLDIER_ATTACKCHECK,
		PLAYER_SOLDIER_BATTLE,
		PLAYER_TANK_MOVE,
		PLAYER_TANK_ATTACKCHECK,
		PLAYER_TANK_BATTLE,
		PLAYER_FLYER_MOVE,
		PLAYER_FLYER_ATTACKCHECK,
		PLAYER_FLYER_BATTLE,
		ENEMY_TURN_START,
		ENEMY_SOLDIER_MOVE,
		ENEMY_SOLDIER_ATTACK,
		ENEMY_TANK_MOVE,
		ENEMY_TANK_ATTACK,
		ENEMY_FLYER_MOVE,
		ENEMY_FLYER_ATTACK,
		PLAYER_WIN,
		PLAYER_LOSE
	};
	State state = PLAYER_SOLDIER_MOVE;

	enum Direction { FRONT, BACK, LEFT, RIGHT, FRONT_LEFT, FRONT_RIGHT, BACK_LEFT, BACK_RIGHT, NONE };
	Direction direction;

	int targetSpaceX, targetSpaceZ;

public:
	TheGame();

	virtual ~TheGame();

	void loadShaders(const string& shaderAssetFilename);

	void setupGraphicsParameters(const string& uniformAssetFilename);

	void setupViewingEnvironment();

	void sendShaderData();

	void setup(const string& gameAssetFilename);

	void updateTextDisplay();

	void processInputs();

	void setColor(int x, int z, Texture* t);

	void showMoveRange(int x, int z, Texture* t);

	void showAttackRange(int x, int z, Texture* t);

	int determineDistance(int x1, int x2, int z1, int z2);

	string determineGridName(int x, int z);

	float determineDirection(int cursorX, int cursorY, int posX, int posY);
	float determineDirectionEnemy(int cursorX, int cursorY, int posX, int posY);

	float determineMoveScale(int cursorX, int cursorZ, int posX, int posZ);
};

#endif

