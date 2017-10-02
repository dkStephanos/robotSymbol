#include "TextFileReader.h"
#include "Logger.h"
#include "RGBA.h"
#include <fstream>
#include <iostream>
using std::ifstream;
using std::ofstream;
using std::getline;
using std::cout;
using std::endl;

TextFileReader::TextFileReader()
{
	this->contents = "";
	this->logger = NULL;
}

TextFileReader::~TextFileReader(void)
{
}

/// <summary>
/// Writes a line to a text file.
/// </summary>
/// <param name="filename">The filename of the text file we are writing to.</param>
/// <param name="line">The string containing what we are writing.</param>
void TextFileReader::writeLine(string filename, string line)
{


	//Open the text file
	ofstream fout;
	fout.open(filename);
	//If we fail, output error
	if (fout.fail())
	{
		cout << "\nThere was an error opening the file." << endl;
		exit(1);
	}
	//If succesful write the 'line'
	fout << line << endl;

	fout.close();

}

/// <summary>
/// Reads the vertex data.
/// </summary>
/// <param name="filename">The filename.</param>
/// <returns></returns>
float* TextFileReader::readVertexData(string filename, string objectName)
{
	string line;
	ifstream fin(filename.c_str());

	//run through .txt file until we reach the object data so we can read it in
	while (line.compare(objectName))
	{
		getline(fin, line);
	}
	//get the count for the vertex array
	getline(fin, line);
	int num = std::stoi(line);
	float parseNum;
	float* vertexData = new float[num];
	vertexData[0] = num;
	//add each line of the text file to the array, where num = array count
	for (int i = 1; i < num; i++)
	{
		getline(fin, line);
		parseNum = std::stof(line);
		vertexData[i] = parseNum;
	}

	return vertexData;
}

/// <summary>
/// Reads the index data.
/// </summary>
/// <param name="filename">The filename.</param>
/// <returns></returns>
short* TextFileReader::readIndexData(string filename, string objectName)
{
	string line = "";
	ifstream fin(filename.c_str());

	//run through .txt file until we reach the object data so we can read it in
	while (line.compare(objectName))
	{
		getline(fin, line);
	}
	//run through .txt file until we reach the index data so we can read it in
	while (line.compare("Index Data:"))
	{
		getline(fin, line);
	}
	//get the count for the vertex array
	getline(fin, line);
	int num = std::stoi(line);
	short parseNum;
	short* indexData = new short[num];
	indexData[0] = num;
	//add each line of the text file to the array, where num = array count
	for (int i = 1; i < num; i++)
	{
		getline(fin, line);
		parseNum = std::stoi(line);
		indexData[i] = parseNum;
	}

	return indexData;
}

const string& TextFileReader::readContents(const string& filename)
{
	this->contents = "";
	string line;
	ifstream fin(filename.c_str());
	if(fin.is_open()){
		while(fin.good()){
			getline(fin, line);
			this->contents += line + "\n";
		}
		fin.close();
	}else{
		if (this->logger) {
			this->logger->log("Cannot open file.");
		}
	}
	return contents;
}

/// <summary>
/// Gets the x coordinate for the game window.
/// </summary>
/// <param name="filename">The filename.</param>
/// <returns></returns>
int TextFileReader::GetX(string filename)
{
	int X = -1;
	ifstream fin;
	fin.open(filename);
	if (fin.fail())
	{
		cout << "\nThere was an error opening the file." << endl;
		exit(1);
	}

	string type, value;
	while (!fin.eof())
	{
		fin >> type >> value;
		if (fin.fail())
		{
			break;
		}
		if (!type.compare("x:"))
		{
			X = std::stoi(value);
			break;
		}
	}
	return X;
}

/// <summary>
/// Gets the y coordinate for the game window.
/// </summary>
/// <param name="filename">The filename.</param>
/// <returns></returns>
int TextFileReader::GetY(string filename)
{
	int Y = -1;
	ifstream fin;
	fin.open(filename);
	if (fin.fail())
	{
		cout << "\nThere was an error opening the file." << endl;
		exit(1);
	}

	string type, value;
	while (!fin.eof())
	{
		fin >> type >> value;
		if (fin.fail())
		{
			break;
		}
		if (!type.compare("y:"))
		{
			Y = std::stoi(value);
			break;
		}
	}
	return Y;
}

/// <summary>
/// Gets the height of the game window.
/// </summary>
/// <param name="filename">The filename.</param>
/// <returns></returns>
float TextFileReader::GetHeight(string filename, string objectName)
{
	float Height = -1;
	ifstream fin;
	fin.open(filename);
	if (fin.fail())
	{
		cout << "\nThere was an error opening the file." << endl;
		exit(1);
	}
	string line;
	//run through .txt file until we reach the index data so we can read it in
	while (line.compare(objectName))
	{
		getline(fin, line);
	}
	string type, value;
	while (!fin.eof())
	{
		fin >> type >> value;
		if (fin.fail())
		{
			break;
		}
		if (!type.compare("height:"))
		{
			Height = std::stof(value);
			break;
		}
	}
	return Height;
}

/// <summary>
/// Gets the width of the game window.
/// </summary>
/// <param name="filename">The filename.</param>
/// <returns></returns>
float TextFileReader::GetWidth(string filename, string objectName)
{
	float Width = -1;
	ifstream fin;
	fin.open(filename);
	if (fin.fail())
	{
		cout << "\nThere was an error opening the file." << endl;
		exit(1);
	}
	string line;
	//run through .txt file until we reach the index data so we can read it in
	while (line.compare(objectName))
	{
		getline(fin, line);
	}
	string type, value;
	while (!fin.eof())
	{
		fin >> type >> value;
		if (fin.fail())
		{
			break;
		}
		if (!type.compare("width:"))
		{
			Width = std::stof(value);
			break;
		}
	}
	return Width;
}

/// <summary>
/// Gets the depth of the game window.
/// </summary>
/// <param name="filename">The filename.</param>
/// <returns></returns>
float TextFileReader::GetDepth(string filename, string objectName)
{
	float Depth = -1;
	ifstream fin;
	fin.open(filename);
	if (fin.fail())
	{
		cout << "\nThere was an error opening the file." << endl;
		exit(1);
	}
	string line;
	//run through .txt file until we reach the index data so we can read it in
	while (line.compare(objectName))
	{
		getline(fin, line);
	}
	string type, value;
	while (!fin.eof())
	{
		fin >> type >> value;
		if (fin.fail())
		{
			break;
		}
		if (!type.compare("depth:"))
		{
			Depth = std::stof(value);
			break;
		}
	}
	return Depth;
}

/// <summary>
/// Gets the title of the game window.
/// </summary>
/// <param name="filename">The filename.</param>
/// <returns></returns>
string TextFileReader::GetTitle(string filename)
{
	string Title;
	ifstream fin;
	fin.open(filename);
	if (fin.fail())
	{
		cout << "\nThere was an error opening the file." << endl;
		exit(1);
	}

	string type, value;
	//clears the file type from the file
	fin >> type;
	while (!fin.eof())
	{
		fin >> type >> value;
		if (fin.fail())
		{
			break;
		}
		if (!type.compare("title:"))
		{
			Title.resize(value.length(), L'\0');
			std::copy(value.begin(), value.end(), Title.begin());
			break;
		}
	}
	return Title;
}

/// <summary>
/// Gets the color of the game window.
/// </summary>
/// <param name="filename">The filename.</param>
/// <returns></returns>
Color TextFileReader::GetColor(string filename)
{
	Color Color;
	Color.r = 0.0f;
	Color.g = 0.0f;
	Color.b = 0.0f;
	Color.a = 0.0f;

	ifstream fin;
	fin.open(filename);
	if (fin.fail())
	{
		cout << "\nThere was an error opening the file." << endl;
		exit(1);
	}

	string type, value1, value2, value3, value4;
	while (!fin.eof())
	{
		fin >> type >> value1 >> value2 >> value3 >> value4;
		if (fin.fail())
		{
			break;
		}
		if (!type.compare("color:"))
		{
			cout << value1 << Color.r;
			cout << value2 << Color.g;
			cout << value3 << Color.b;
			cout << value4 << Color.a;
			break;
		}
	}
	return Color;
}

/// <summary>
/// Gets the color of the game window.
/// </summary>
/// <param name="filename">The filename.</param>
/// <returns></returns>
RGBA TextFileReader::GetRGBA(string filename, string objectName)
{
	RGBA Color;
	Color.red = 0.0f;
	Color.green = 0.0f;
	Color.blue = 0.0f;
	Color.alpha = 0.0f;

	ifstream fin;
	fin.open(filename);
	if (fin.fail())
	{
		cout << "\nThere was an error opening the file." << endl;
		exit(1);
	}
	string line;
	//run through .txt file until we reach the index data so we can read it in
	while (line.compare(objectName))
	{
		getline(fin, line);
	}
	string type, value1, value2, value3, value4;
	while (!fin.eof())
	{
		fin >> type;
		if (fin.fail())
		{
			break;
		}
		if (!type.compare("color:"))
		{
			fin >> value1 >> value2 >> value3 >> value4;
			Color.red = std::stof(value1);
			Color.green = std::stof(value2);
			Color.blue = std::stof(value3);
			Color.alpha = std::stof(value4);
			break;
		}
	}
	return Color;
}