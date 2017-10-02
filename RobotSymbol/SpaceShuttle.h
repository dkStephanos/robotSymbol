#pragma once
#include "OGL3DObject.h"
#include <string>
using std::string;

/// <summary>
/// Provides methods and attributes required for a space shuttle object.
/// </summary>
class SpaceShuttle :
	public OGL3DObject
{
protected:
	void update(float elapsedTimeMS);

public:
	SpaceShuttle(const string& name, float scaler);
	~SpaceShuttle();

	void launch();

	void generate(float scaler);
};

