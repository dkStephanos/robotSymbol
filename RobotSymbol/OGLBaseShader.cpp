#include "OGLBaseShader.h"
#include "Logger.h"

#include <cstring>


OGLBaseShader::OGLBaseShader() : BaseShader()
{
	this->handle = 0;
}

OGLBaseShader::~OGLBaseShader()
{
	auto iterator = this->uniforms.begin();
	while (iterator != this->uniforms.end()) {
		delete iterator->second;
		iterator++;
	}
}

void OGLBaseShader::addUniform(string uniformName, UniformDataType type)
{
	GLuint location = glGetUniformLocation(
		this->handle,
		uniformName.c_str()
	);
	if (this->uniforms.find(uniformName) == this->uniforms.end()) {
		this->uniforms[uniformName] = new Uniform { location, type, NULL };
	}
}

void OGLBaseShader::setUniformData(const string& uniformName, void *data)
{
	Uniform* uniform = this->getUniform(uniformName);
	if (uniform) {
		uniform->data = data;
	}
}

void OGLBaseShader::sendData(const string& uniformName)
{
	Uniform* uniform = this->getUniform(uniformName);
	if(uniform){
		switch (uniform->type) {
		case Integer:
			this->sendInteger(uniform->location, uniform->data);
			break;
		case Float:
			this->sendFloat(uniform->location, uniform->data);
			break;
		case FloatArray:
			this->sendFloatArray(uniform->location, uniform->data, 10);
			break;
		case Matrix4:
			this->sendMatrix4(uniform->location, uniform->data);
			break;
		case Vector3:
			this->sendVector3(uniform->location, uniform->data);
			break;
		case Vector3Array:
			this->sendVector3Array(uniform->location, uniform->data, 10);
			break;
		case Vector4:
			this->sendVector4(uniform->location, uniform->data);
			break;
		case Sampler2D:
			this->sendSampler2D(uniform->location, uniform->data);
			break;
		}
	}
}

void OGLBaseShader::sendMatrix4(GLuint location, void *matrix)
{
	glUseProgram(this->handle);
	glUniformMatrix4fv(location, 1, GL_FALSE, (float *)matrix);
	glUseProgram(0);
}

void OGLBaseShader::sendVector3(GLuint location, void *vector)
{
	glUseProgram(this->handle);
	glUniform3fv(location, 1, (float *)vector
	);
	glUseProgram(0);
}

void OGLBaseShader::sendVector4(GLuint location, void *vector)
{
	glUseProgram(this->handle);
	glUniform4fv(location, 1, (float *)vector);
	glUseProgram(0);
}

void OGLBaseShader::sendVector3Array(GLuint location, void *vectorArray, unsigned int count)
{
	glUseProgram(this->handle);
	glUniform3fv(location, count, (float*)vectorArray);
	glUseProgram(0);
}

void OGLBaseShader::sendInteger(GLuint location, void *value)
{
	glUseProgram(this->handle);
	glUniform1i(location, int(*(int*)value));
	glUseProgram(0);
}

void OGLBaseShader::sendFloat(GLuint location, void *value)
{
	glUseProgram(this->handle);
	glUniform1f(location, float(*(float*)value));
	glUseProgram(0);
}

void OGLBaseShader::sendSampler2D(GLuint location, void *value)
{
	this->sendInteger(location, value);
}

void OGLBaseShader::sendFloatArray(GLuint location, void *values, unsigned int count)
{
	glUseProgram(this->handle);
	glUniform1fv(location, count, (float *)values);
	glUseProgram(0);
}

Uniform* OGLBaseShader::getUniform(const string& uniformName)
{
	Uniform* uniform = NULL;
	if (this->uniforms.find(uniformName) != this->uniforms.end()) {
		uniform = this->uniforms[uniformName];
	}
	return uniform;
}

void OGLBaseShader::showInfoLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
	GLint logLength;
	glGet__iv(object, GL_INFO_LOG_LENGTH, &logLength);
	char* log = (char*)malloc(logLength);
	glGet__InfoLog(object, logLength, NULL, log);
	if (this->logger) {
		this->logger->log(string(log));
	}
	free(log);
}