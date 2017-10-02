#pragma once
#include "OGL3DObject.h"
#include <string>
using std::string;

/// <summary>
/// Provides the specific methods required for a MountainRange object.
/// </summary>
class MountainRange :
	public OGL3DObject
{
protected:
	void update(float elapsedTimeMS);

public:
	MountainRange(const string& name, float scaler);
	~MountainRange();
	void generate(float scaler);
};

