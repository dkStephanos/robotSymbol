#pragma once
#ifndef WINDOWS_CONSOLE_LOGGER
#define WINDOWS_CONSOLE_LOGGER

#include <Windows.h>
#include <string>
using std::wstring;
using std::string;

#include "Logger.h"

#define _LOGGING_

class WindowsConsoleLogger :
	public Logger
{
private:
	HANDLE stdOut;
	wchar_t *buffer;
	int size;

public:
	static const int MAX_CONSOLE_LINES = 500;
	static const int BUFFER_SIZE = 256;

public:
	WindowsConsoleLogger(void);
	~WindowsConsoleLogger(void);

	void log(string text);
	void log(wstring text);
	void debug(string text);
	void debug(wstring text);

private:
	void create();
	void write();
};

typedef WindowsConsoleLogger WCLogger;

#endif

