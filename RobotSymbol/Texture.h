#pragma once
#ifndef TEXTURE
#define TEXTURE

class Texture
{
protected:
	void* data;
	unsigned int width, height;

public:
	Texture();

	virtual ~Texture();

	void set(void* data, unsigned int width, unsigned int height);

	void* getData() { return this->data; }

	unsigned int getHeight() { return this->height; }

	unsigned int getWidth() { return this->width; }

	virtual void create() {};

	virtual void select() {};
};

#endif

