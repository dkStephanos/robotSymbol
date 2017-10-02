#pragma once
#ifndef CORE_SYSTEM
#define CORE_SYSTEM

class TextFileReader;
class Logger;

class CoreSystem
{
protected:
	TextFileReader *textFileReader;
	Logger *logger;

public:
	CoreSystem(TextFileReader *textFileReader);
	virtual ~CoreSystem();

	void setLogger(Logger* logger);
	TextFileReader *getTextFileReader() { return this->textFileReader; }
};

#endif

