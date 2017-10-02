#pragma once
#ifndef LOGGER
#define LOGGER

#include <string>
using std::string;
using std::wstring;

class Logger
{
public:
	Logger();
	virtual ~Logger();

	virtual void log(string text) = 0;
	virtual void log(wstring text) = 0;
	virtual void debug(string text) = 0;
	virtual void debug(wstring text) = 0;
};

#endif

