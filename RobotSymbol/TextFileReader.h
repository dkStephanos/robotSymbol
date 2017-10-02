#pragma once
#ifndef TEXT_FILE_READER
#define TEXT_FILE_READER

#include <string>
using std::string;
#include <vector>
#include "RGBA.h"
using std::vector;

class Logger;

//Struct to hold the color information for the window
struct Color
{
	float r;
	float g;
	float b;
	float a;
};
/**
  Has the responsibility of reading data from a text file
*/
class TextFileReader
{
private:
	// The file's content as a string
	string contents;
	// The logger
	Logger * logger;

public:
	// Constructor
	TextFileReader();
	
	// Destructor
	~TextFileReader(void);

	// Reads the contents of the file and returns it as a single string
	// Params:
	//    filename - the name of the from which data will be read
	// Returns:
	//    the contents of the file
	const string& readContents(const string& filename);

	// Getter
	string getContexts() const { return contents; }

	// Sets the logger
	void setLogger(Logger * logger) { this->logger = logger;  }

	//These methods obtain the configuration data from text files
	int GetX(string filename);
	int GetY(string filename);
	float GetHeight(string filename, string objectName);
	float GetWidth(string filename, string objectName);
	float GetDepth(string filename, string objectName);
	Color GetColor(string filename);
	RGBA GetRGBA(string filename, string objectName);
	string GetTitle(string filename);

	//Used to write and obtain data for game objects to and from game files respectively. 
	void writeLine(string filename, string line);
	float* TextFileReader::readVertexData(string filename, string objectName);
	short* TextFileReader::readIndexData(string filename, string objectName);
};

#endif

