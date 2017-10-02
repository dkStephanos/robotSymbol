#pragma once
#ifndef BASE_OBJECT
#define BASE_OBJECT

#include <glm/glm.hpp>

#include <string>
using std::string;

class Logger;

class BaseObject
{
protected:
	Logger* logger;

	string name;

public:
	BaseObject(Logger* logger=nullptr);

	virtual ~BaseObject();

	virtual void setLogger(Logger* logger) { this->logger = logger; }

	virtual Logger* getLogger() { return this->logger; }

	virtual void setName(const string& name) { this->name = name; }

	virtual const string& getName()  const { return this->name; }

};

#endif

