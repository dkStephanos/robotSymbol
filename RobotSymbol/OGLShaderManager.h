#pragma once
#ifndef OGL_SHADER_MANAGER
#define OGL_SHADER_MANAGER

#include <string>
using std::string;
#include <map>
using std::map;

#include <gl\glew.h>
#include <glm\glm.hpp>

#include "OGLShaderProgram.h"
#include "ShaderManager.h"
#include "Uniform.h"
#include "MathUtil.h"

class OGLShaderCompiler;

struct ShadingUnit {
	OGLShaderProgram * shaderProgram;
	string vertexShaderName;
	string fragmentShaderName;
};

class OGLShaderManager : public ShaderManager
{
protected:
	map<string, OGLShaderCompiler *> vertexShaders;
	map<string, OGLShaderCompiler *> fragmentShaders;
	map<string, ShadingUnit> shadingUnits;

public:
	OGLShaderManager();

	~OGLShaderManager();

	void addVertexShader(string name, OGLShaderCompiler* vertexShader);

	void addFragmentShader(string name, OGLShaderCompiler* fragmentShader);

	void addShaderProgram(
		string name, 
		OGLShaderProgram* shaderProgram, 
		string vertexShaderName, 
		string fragmentShaderName);

	void addUniform(string uniformName, UniformDataType type, const string& shaderName);

	Uniform* getUniform(const string& uniformName, const string& shaderName);

	bool createShader(const string& shaderName);

	bool createShaders();

	GLuint getShaderHandle(string shaderName);

	BaseShader* getShader(const string& shaderName);

private:
	bool compileVertexShader(string vertexShaderName);
	bool compileFragmentShader(string fragmentShaderName);
	bool linkShader(string shaderName, OGLShaderCompiler * vertexShader, OGLShaderCompiler * fragmentShader);
};

#endif

