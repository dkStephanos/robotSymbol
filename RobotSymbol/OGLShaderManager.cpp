#include "OGLShaderManager.h"
#include "OGLShaderCompiler.h"
#include "Logger.h"
#include "Uniform.h"

#include <glm\gtc\type_ptr.hpp>

OGLShaderManager::OGLShaderManager()
{
}

OGLShaderManager::~OGLShaderManager()
{
}

GLuint OGLShaderManager::getShaderHandle(string shaderName)
{
	if (this->shadingUnits.find(shaderName) != this->shadingUnits.end()) {
		return this->shadingUnits[shaderName].shaderProgram->getHandle();
	}
	return 0;
}

BaseShader* OGLShaderManager::getShader(const string& shaderName)
{
	if (this->shadingUnits.find(shaderName) != this->shadingUnits.end()) {
		return this->shadingUnits[shaderName].shaderProgram;
	}
	return NULL;
}

void OGLShaderManager::addVertexShader(string name, OGLShaderCompiler* vertexShader)
{
	if (this->vertexShaders.find(name) == this->vertexShaders.end()) {
		this->vertexShaders[name] = vertexShader;
	}
}

void OGLShaderManager::addFragmentShader(string name, OGLShaderCompiler* fragmentShader)
{
	if (this->fragmentShaders.find(name) == this->fragmentShaders.end()) {
		this->fragmentShaders[name] = fragmentShader;
	}
}

void OGLShaderManager::addShaderProgram(string name, OGLShaderProgram* shaderProgram, string vertexShaderName, string fragmentShaderName)
{
	if (this->shadingUnits.find(name) == this->shadingUnits.end()) {
		ShadingUnit unit = { shaderProgram, vertexShaderName, fragmentShaderName };
		this->shadingUnits[name] = unit;
	}
}

void OGLShaderManager::addUniform(string uniformName, UniformDataType type, const string& shaderName)
{
	if (this->shadingUnits.find(shaderName) == this->shadingUnits.end()) {
		return;
	}

	OGLShaderProgram* shaderProgram = this->shadingUnits[shaderName].shaderProgram;
	shaderProgram->addUniform(uniformName, type);
}

Uniform* OGLShaderManager::getUniform(const string& uniformName, const string& shaderName)
{
	Uniform* uniform = NULL;
	if (this->shadingUnits.find(shaderName) != this->shadingUnits.end()) {
		OGLShaderProgram* shaderProgram = this->shadingUnits[shaderName].shaderProgram;
		uniform = shaderProgram->getUniform(uniformName);
	}
	return uniform;
}

bool OGLShaderManager::createShaders()
{
	bool result;
	for (auto iterator = this->shadingUnits.begin(); iterator != this->shadingUnits.end(); iterator++) {
		result = this->createShader(iterator->first);
		if (!result) return false;
	}
	return true;
}

bool OGLShaderManager::createShader(const string& shaderName)
{
	string vertexShaderName = this->shadingUnits[shaderName].vertexShaderName;
	bool result = this->compileVertexShader(vertexShaderName);
	if (!result) return false;

	string fragmentShaderName = this->shadingUnits[shaderName].fragmentShaderName;
	result = this->compileFragmentShader(fragmentShaderName);
	if (!result) return false;

	return this->linkShader(
		shaderName, 
		this->vertexShaders[vertexShaderName], 
		this->fragmentShaders[fragmentShaderName]);
}

bool OGLShaderManager::compileVertexShader(string vertexShaderName)
{
	bool result = false;
	OGLShaderCompiler* vertexShader = NULL;
	if (this->vertexShaders.find(vertexShaderName) != this->vertexShaders.end()) {
		vertexShader = this->vertexShaders[vertexShaderName];
		if (vertexShader->getHandle() == 0) {
			result = vertexShader->compile();
		}
		else {
			result = true;
		}
	}
	return result;
}

bool OGLShaderManager::compileFragmentShader(string fragmentShaderName)
{
	bool result = false;
	OGLShaderCompiler* fragmentShader = NULL;
	if (this->fragmentShaders.find(fragmentShaderName) != this->fragmentShaders.end()) {
		fragmentShader = this->fragmentShaders[fragmentShaderName];
		if (fragmentShader->getHandle() == 0) {
			result = fragmentShader->compile();
		}
		else {
			result = true;
		}
	}
	return result;
}

bool OGLShaderManager::linkShader(
	string shaderName, OGLShaderCompiler * vertexShader, OGLShaderCompiler * fragmentShader)
{
	bool result = false;
	OGLShaderProgram* shaderProgram = NULL;
	if (this->shadingUnits.find(shaderName) != this->shadingUnits.end()) {
		shaderProgram = this->shadingUnits[shaderName].shaderProgram;
		result = shaderProgram->link(vertexShader->getHandle(), fragmentShader->getHandle());
	}
	return result;
}


