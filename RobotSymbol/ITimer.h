#pragma once
#ifndef TIMER
#define TIMER

#ifndef ABSTRACT
#define ABSTRACT 0
#endif

/**
The Timer interface specifies the timing protocol used in this game engine.
**/
class ITimer
{
public:
	// The constructor
	ITimer(void);
	// The destructor
	virtual ~ITimer(void);

public:
	// Method startTiming should reset the timing counter.
	virtual void startTiming() = ABSTRACT;

	// Method stop() should reset the elapsed time counter and calculate the duration
	// since timing started.
	virtual void stop() = ABSTRACT;

	// Method getDuration() should return the duration since the timing started in milliseconds
	virtual float getDuration() const = ABSTRACT;

	// Method getDurationInSeconds() should return the duration since the timing started in seconds
	virtual float getDurationInSeconds() const = ABSTRACT;

	// Method isHighResolution should return true if this timer is a high resolution
	// timer.
	virtual bool isHighResolution() = ABSTRACT;

	// getElapsedTimeInSeconds() should return the number of seconds since the last
	// time it was called.
	virtual float getElapsedTimeInSeconds() = ABSTRACT;

	// getFPS should return the average number of frames per second given the 
	// number of elapsed frames.  The default number of elapsed frames is
	// one.
	virtual float getFPS(unsigned long elapsedFrames = 1) = ABSTRACT;

	// lockFPS should lock the number of frames per second to the target FPS, this
	// method can only slow down the number of FPS, it cannot speed it up.
	virtual float lockFPS(unsigned long targetFPS) = ABSTRACT;

	// Should return the current time in seconds
	virtual float getCurrentTimeInSeconds() = ABSTRACT;

	// Should return the current time in milliseconds
	virtual float getCurrentTimeInMilliseconds() = ABSTRACT;
};

#endif

