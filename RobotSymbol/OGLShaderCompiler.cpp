#include "OGLShaderCompiler.h"
#include "Logger.h"

#include <cstring>

OGLShaderCompiler::OGLShaderCompiler(string sourceCode) : OGLBaseShader()
{
	this->sourceCode = sourceCode;
	this->handle = 0;
}

OGLShaderCompiler::~OGLShaderCompiler()
{
	if (this->handle != 0) {
		glDeleteShader(this->handle);
	}
}

void OGLShaderCompiler::compile(GLenum type)
{
	const GLchar* source = this->sourceCode.c_str();
	GLint length = sizeof(GLchar) * strlen(source);
	this->handle = glCreateShader(type);
	glShaderSource(this->handle, 1, (const GLchar**)&source, &length);
	glCompileShader(this->handle);
	GLint shaderOk = 0;
	glGetShaderiv(this->handle, GL_COMPILE_STATUS, &shaderOk);
	if (!shaderOk) {
		if (this->logger) {
			this->logger->log("makeShader: Failed to compile the shader!");
		}
		showInfoLog(this->handle, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(this->handle);
		this->handle = 0;
	}
}
