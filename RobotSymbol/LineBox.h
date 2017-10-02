#pragma once
#ifndef LINE_BOX
#define LINE_BOX

#include "BaseCuboid.h"

class LineBox :	public BaseCuboid
{
public:
	LineBox(
		const string& name, 
		float width = 1, float depth = 1, float height = 1, 
		RGBA lineColor = { 1, 1, 1, 1 });

	virtual ~LineBox();

protected:
	void generate();
};

#endif

