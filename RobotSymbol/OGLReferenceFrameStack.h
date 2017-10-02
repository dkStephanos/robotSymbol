#pragma once
#ifndef OGL_REFERENCE_FRAME_STACK
#define OGL_REFERENCE_FRAME_STACK

#include <stack>
#include <glm/glm.hpp>
#include "OGLReferenceFrame.h"

class OGLReferenceFrameStack
{
private:
	// The current frame i.e. typically the top of the stack
	OGLReferenceFrame currFrame;
	// The stack of frames
	std::stack<OGLReferenceFrame> frames;

public:
	OGLReferenceFrameStack(void);

	~OGLReferenceFrameStack(void);

	// Sets the base frame to the stack i.e. the top
	// Param:
	//    frame - the frame
	inline void setBaseFrame(OGLReferenceFrame frame){
		this->currFrame = frame;
	}

	// Returns the top of the stack i.e. the current frame
	inline OGLReferenceFrame top() { return this->currFrame; }

	// Rotates the top frame about the x axis
	inline void rotateX(float degrees) { this->currFrame.rotateX(degrees); }

	// Rotates the top frame about the y axis
	inline void rotateY(float degrees) { this->currFrame.rotateY(degrees); }

	// Rotates the top frame about the z axis
	inline void rotateZ(float degrees) { this->currFrame.rotateZ(degrees); }

	// Translates the top frame by x, y, z
	inline void translate(float x, float y, float z) { this->currFrame.translate(x, y, z); }

	// Pops the top of the stack
	OGLReferenceFrame pop();

	// Pushes the current frame to the stack
	void push();

	void applyTransformsFrom(const OGLReferenceFrame& frame);
};

#endif

