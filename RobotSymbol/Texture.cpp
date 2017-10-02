#include "Texture.h"

Texture::Texture()
{
	this->data = nullptr;
	this->width = this->height = 0;
}

Texture::~Texture()
{
	if (this->data) {
		delete[] this->data;
	}
}

void Texture::set(void* data, unsigned int width, unsigned int height)
{
	this->data = data;
	this->width = width;
	this->height = height;
}
