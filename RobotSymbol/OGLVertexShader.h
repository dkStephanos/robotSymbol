#pragma once
#ifndef OGL_VERTEX_SHADER
#define OGL_VERTEX_SHADER

#include "OGLShaderCompiler.h"

#include <string>
using std::string;

#ifndef NULL
#define NULL 0
#endif

class OGLVertexShader : public OGLShaderCompiler
{

public:
	OGLVertexShader(string source="");
	virtual ~OGLVertexShader();

	bool compile();

	static string stockSource();
};

#endif

