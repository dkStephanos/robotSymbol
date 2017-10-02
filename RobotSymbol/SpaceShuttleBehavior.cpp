#include "SpaceShuttleBehavior.h"
#include "OGLObject.h"
#include "OGL3DObject.h"


/// <summary>
/// Initializes a new instance of the <see cref="SpaceShuttleBehavior"/> class.
/// </summary>
/// <param name="maxDistance">The maximum distance.</param>
SpaceShuttleBehavior::SpaceShuttleBehavior(float maxDistance) : IBehavior(object)
{
	this->state = WAIT;
	this->maxDistance = maxDistance;
	this->distanceMoved = 0;
}


/// <summary>
/// Finalizes an instance of the <see cref="SpaceShuttleBehavior"/> class.
/// </summary>
SpaceShuttleBehavior::~SpaceShuttleBehavior()
{
}

/// <summary>
/// Makes the object wait until lauch method is called by FlyingRobot object.
/// Then moves from moving state up to moving forward state in which the object
/// rotates as it rises and then flys out of the scene.
/// </summary>
/// <param name="object">The object.</param>
/// <param name="elapsedSeconds">The elapsed seconds.</param>
void SpaceShuttleBehavior::update(GameObject *object, float elapsedSeconds)
{


}