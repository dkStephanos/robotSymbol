#pragma once
#ifndef WINDOWS_TIMER
#define WINDOWS_TIMER

#include <Windows.h>
#include "ITimer.h"

/**
	Created by: Jeff Roach Jan 29, 2006 (ETSU)
	Based on wrapper class created by Dave Astle 2/1/2001
	Needs: winmm.lib
**/
class WindowsTimer : public ITimer
{
protected:
	// LARGE_INTEGER Documentation: 
	// http://msdn.microsoft.com/en-us/library/aa383713%28v=vs.85%29.aspx (Feb 15, 2013)

	// Stores the number of ticks per second
	LARGE_INTEGER ticksPerSecond;
	// Used for timing
	LARGE_INTEGER start, finish;
	// Stores whether or not this timer is high resolution
	bool highResolutionFlag;
	// Stores the time in milliseconds between calling startTiming() and stop()
	float duration;

public:
	WindowsTimer(void);
	virtual ~WindowsTimer(void);

	// Method startTiming resets the timing counter.
	void startTiming();

	// Method stop() resets the elapsed time counter and calculates the duration
	// since timing started.
	void stop();

	// Getter
	float getDuration() const { return duration; }

	// Getter
	float getDurationInSeconds() const { return (duration / ticksPerSecond.QuadPart); }

	// Method isHighResolution returns true if this timer is a high resolution
	// timer.
	bool isHighResolution() { return highResolutionFlag; }

	// getElapsedTimeInSeconds() returns the number of seconds since the last
	// time getElapsedTimeInSeconds() was called.
	float getElapsedTimeInSeconds();

	// getFPS returns the average number of frames per second given the 
	// number of elapsed frames.  The default number of elapsed frames is
	// one.
	float getFPS(unsigned long elapsedFrames = 1);

	// lockFPS locks the number of frames per second to the target FPS, this
	// method can only slow down the number of FPS, it cannot speed it up.
	// Used to lock the frame rate to a set amount. This will block until enough
	// time has passed to ensure that the fps won't go over the requested amount.
	// Note that this can only keep the fps from going above the specified level;
	// it can still drop below it. It is assumed that if used, this function will
	// be called every frame. The value returned is the instantaneous fps, which
	// will be <= targetFPS.
	float lockFPS(unsigned long targetFPS);

	// If the high resolution timer is present, the current time is stored in high
	// resolution, otherwise a low-res value is returned.
	float getCurrentTimeInSeconds();

	// Should return the current time in milliseconds
	float getCurrentTimeInMilliseconds();


private:
	// The getCurrentTime method stores the current time into the given
	// parameter.
	void getCurrentTime(LARGE_INTEGER& currentTime);

	// The initialize method is called by the constructor, and it establishes
	// whether or not the timer is high-resolution.  It also establishes the
	// frequency, that is, the number of clock ticks per second.
	void initialize();

};

#endif

