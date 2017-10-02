#pragma once
#ifndef CUSTOM_TEXTURE
#define CUSTOM_TEXTURE

#include "Texture.h"

#include <gl\glew.h>

class CustomTexture :
	public Texture
{
protected:
	GLuint id;

public:
	CustomTexture();
	virtual ~CustomTexture();

	void create();

	void select();
};

#endif

