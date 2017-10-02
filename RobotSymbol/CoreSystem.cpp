#include "CoreSystem.h"
#include "TextFileReader.h"
#include "Logger.h"

CoreSystem::CoreSystem(TextFileReader *textFileReader)
{
	this->textFileReader = textFileReader;
}

CoreSystem::~CoreSystem()
{
	delete textFileReader;
}

void CoreSystem::setLogger(Logger* logger)
{ 
	this->logger = logger; 
	this->textFileReader->setLogger(this->logger);
}
