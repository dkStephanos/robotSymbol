#pragma once
#ifndef OGL_SHADER_COMPILER
#define OGL_SHADER_COMPILER

#include "OGLBaseShader.h"

#include <string>
using std::string;

class OGLShaderCompiler :
	public OGLBaseShader
{
protected:
	string sourceCode;

public:
	OGLShaderCompiler(string sourceCode = "");
	virtual ~OGLShaderCompiler();

	virtual bool compile() = 0;

	void setSource(string source) { this->sourceCode = source; }

protected:
	void compile(GLenum type);
};

#endif

