#include "BMPTextureLoader.h"
#include "Texture.h"
#include <fstream>
using namespace std;

BMPTextureLoader::BMPTextureLoader()
{
	this->filepath = "";
	this->status = "Ok";
}


BMPTextureLoader::~BMPTextureLoader()
{
}

Texture* BMPTextureLoader::load()
{
	ifstream fin;
	fin.open(this->filepath.c_str(), ios::in | ios::binary);
	if (fin.fail()) {
		this->status = "There was a problem opening " + this->filepath;
		return nullptr;
	}
	// Read the BMP's header
	const int BmpHeaderSize = 54;
	char header[BmpHeaderSize];
	fin.read(header, BmpHeaderSize);
	if (fin.fail()) {
		this->status = this->filepath + " is not a BMP file. ";
		return nullptr;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		this->status = this->filepath + " is not a BMP file. ";
		return nullptr;
	}
	int dataPos = *(int*)&(header[10]);
	int imageWidth = *(int*)&(header[18]);
	int imageHeight = *(int*)&(header[22]);
	int imageSize = *(int*)&(header[34]);
	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0) {
		// 3 : one byte for each Red, Green and Blue component
		imageSize = imageWidth * imageHeight * 3;
	}
	if (dataPos == 0) {
		dataPos = BmpHeaderSize;
	}
	fin.seekg(dataPos);
	char* data = new char[imageSize];
	fin.read(data, imageSize);
	if (fin.fail()) {
		this->status = "There was a problem reading image data from " + this->filepath;
		return nullptr;
	}

	Texture* texture = new Texture();
	texture->set(reinterpret_cast<unsigned char*>(data), imageWidth, imageHeight);
	return texture;
}
