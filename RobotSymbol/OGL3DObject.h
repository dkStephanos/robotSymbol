#pragma once
#ifndef OGL_3D_OBJECT
#define OGL_3D_OBJECT

#include "OGLObject.h"

#include <string>
using std::string;

class Axis;

class OGL3DObject :
	public OGLObject
{
protected:
	string uniformMatrixName;

public:
	OGL3DObject(const string& name, const string& uniformMatrixName="localToWorldMatrix");

	virtual ~OGL3DObject();

	void setUniformMatrixName(const string& name) { 
		this->uniformMatrixName = name; 
	}

	void render();

	void render(OGLReferenceFrame& frame);

protected:
	void sendMatrixToGPU(const glm::mat4& mat);
};

#endif

