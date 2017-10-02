#pragma once
#ifndef SHADER_MANAGER
#define SHADER_MANAGER

#include "BaseObject.h"

#include <string>
using std::string;

class Logger;
class BaseShader;

class ShaderManager : public BaseObject
{
public:
	ShaderManager();

	virtual ~ShaderManager();

	virtual bool createShader(const string& shaderName) = 0;

	virtual BaseShader* getShader(const string& shaderName) = 0;
};

#endif

