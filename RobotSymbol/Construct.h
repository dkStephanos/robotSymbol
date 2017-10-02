#pragma once
#ifndef CONSTRUCT
#define CONSTRUCT

#include "OGL3DCompositeObject.h"

#include <unordered_map>
using std::unordered_map;

#include <string>
using std::string;

class OGLTexturedFlatSurface;

class Construct : public OGL3DCompositeObject
{
public:
	unordered_map<string, OGLTexturedFlatSurface*> surfaces;

public:
	Construct(const string& name);
	virtual ~Construct();

	void setShaderProgram(GLuint shaderProgram);

	void update(float elapsedSeconds);

	void render();
};

#endif

