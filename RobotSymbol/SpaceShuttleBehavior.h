#pragma once
#include "IBehavior.h"
#include "OGL3DObject.h"

/// <summary>
/// Provides variables and states required for implementing a Space shuttle behavior
/// in which the object rises while rotating and then moves forward out of the scene.
/// </summary>
class SpaceShuttleBehavior :
	public IBehavior
{
public:
	/// <summary>
	/// The various states associated with waiting for, and launching the space shuttle
	/// </summary>
	enum State { MOVING_UP, MOVING_FORWARD, WAIT };

protected:
	State state;
	float maxDistance;
	float distanceMoved;

public:
	SpaceShuttleBehavior(float maxDistance);
	virtual ~SpaceShuttleBehavior();

	void update(GameObject *object, float elapsedSeconds);

	void setState(State state) { this->state = state; }
};

