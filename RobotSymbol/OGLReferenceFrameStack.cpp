#include "OGLReferenceFrameStack.h"
#include "MathUtil.h"

OGLReferenceFrameStack::OGLReferenceFrameStack()
{
}

OGLReferenceFrameStack::~OGLReferenceFrameStack()
{
}

OGLReferenceFrame OGLReferenceFrameStack::pop()
{
	this->currFrame = this->frames.top();
	this->frames.pop();
	return this->currFrame;
}

void OGLReferenceFrameStack::push()
{
	this->frames.push(currFrame);
}

void OGLReferenceFrameStack::applyTransformsFrom(const OGLReferenceFrame& frame)
{
	for (size_t i = 0; i < frame.transforms.size(); i++) {
		if (frame.transforms[i].type == "TL") {
			this->currFrame.translate(
				frame.transforms[i].x, 
				frame.transforms[i].y, 
				frame.transforms[i].z);
		}
		else if(frame.transforms[i].type == "RLx") {
			this->currFrame.rotateX(frame.transforms[i].x);
		}
		else if (frame.transforms[i].type == "RLz") {
			this->currFrame.rotateZ(frame.transforms[i].z);
		}
		else if (frame.transforms[i].type == "RLy") {
			this->currFrame.rotateY(frame.transforms[i].y);
		}
	}
}
