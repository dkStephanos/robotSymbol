#pragma once
#ifndef OGL_SHADER_PROGRAM
#define OGL_SHADER_PROGRAM

#include "OGLBaseShader.h"

#include <gl/glew.h>

class OGLShaderProgram : public OGLBaseShader
{
public:
	OGLShaderProgram();
	virtual ~OGLShaderProgram();

	bool link(GLuint vertexHandle, GLuint fragmentHandle);
};

#endif

