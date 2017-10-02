#include "OGLFragmentShader.h"

OGLFragmentShader::OGLFragmentShader(string sourceCode) : OGLShaderCompiler(sourceCode)
{
	if (this->sourceCode == "") {
		this->sourceCode = OGLFragmentShader::stockSource();
	}
}

OGLFragmentShader::~OGLFragmentShader()
{
}

bool OGLFragmentShader::compile()
{
	OGLShaderCompiler::compile(GL_FRAGMENT_SHADER);
	if (this->handle == 0) return false;
	return true;
}

string OGLFragmentShader::stockSource()
{
	static string fragmentSource =
		"#version 330\n"\
		"in vec4 fragColor;\n"\
		"out vec4 color;\n"\
		"void main()\n"\
		"{\n"\
		"   color = fragColor;\n"\
		"}\n";
	return fragmentSource;
}