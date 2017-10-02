#include "AssetLoader.h"

#include <fstream>
using std::ifstream;
#include <sstream>
using std::stringstream;

AssetLoader::AssetLoader() : BaseObject()
{
	this->gameEngine = nullptr;
	this->ci = 0;
}

AssetLoader::~AssetLoader()
{
}

void AssetLoader::loadAssetFile(const string& filepath)
{
	if (filepath == "") return;
	this->lines.clear();
	this->ci = 0;

	ifstream fin;
	fin.open(filepath.c_str());
	if (fin.fail()) return;

	string line;
	while (!fin.eof()) {
		getline(fin, line);
		this->trim(line);
		if (line != "") {
			if (line[0] != '!') {
				this->lines.push_back(line);
			}
		}
	}
	fin.close();
}

string AssetLoader::nextLine()
{
	this->ci++;
	if (this->ci >= this->lines.size()) {
		this->ci = this->lines.size() - 1;
	}
	return this->lines[this->ci];
}

void AssetLoader::loadElement(vector<string>& fields, const string& end, char delimeter)
{
	while (!this->atEnd() && this->nextLine() != end) {
		this->split(fields, this->currLine(), delimeter);
	}
}

void AssetLoader::split(vector<string>& fields, const string& record, char delimeter)
{
	stringstream ss;
	ss << record;
	string field;
	while (getline(ss, field, delimeter))
	{
		this->trim(field);
		fields.push_back(field);
	}
}

void AssetLoader::trim(string& str)
{
	const string delimiters = " \f\n\r\t\v";
	str.erase(str.find_last_not_of(delimiters) + 1);
	str.erase(0, str.find_first_not_of(delimiters));
}