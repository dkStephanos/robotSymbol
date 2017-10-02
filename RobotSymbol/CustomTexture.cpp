#include "CustomTexture.h"

CustomTexture::CustomTexture()
{
}

CustomTexture::~CustomTexture()
{
}

void CustomTexture::create()
{
	glGenTextures(1, &this->id);
	this->select();
	float data[] = {
		1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.01f,  0.0f, 0.0f, 0.01f,  1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.01f,  0.0f, 0.0f, 0.01f,  1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.01f, 1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.01f
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void CustomTexture::select()
{
	glBindTexture(GL_TEXTURE_2D, this->id);
}
