#include "OGLShaderProgram.h"
#include "Logger.h"

OGLShaderProgram::OGLShaderProgram() : OGLBaseShader()
{
}

OGLShaderProgram::~OGLShaderProgram()
{
}

bool OGLShaderProgram::link(GLuint vertexHandle, GLuint fragmentHandle)
{
	this->handle = glCreateProgram();
	glAttachShader(this->handle, vertexHandle);
	glAttachShader(this->handle, fragmentHandle);
	glLinkProgram(this->handle);
	GLint programOk = 0;
	glGetProgramiv(this->handle, GL_LINK_STATUS, &programOk);
	if (!programOk) {
		if (this->logger) {
			this->logger->log("linkShader: Failed to link the shader!");
		}
		OGLBaseShader::showInfoLog(this->handle, glGetProgramiv, glGetProgramInfoLog);
		glDeleteShader(this->handle);
		this->handle = 0;
		return false;
	}
	return true;
}