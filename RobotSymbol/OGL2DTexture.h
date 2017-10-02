#pragma once
#ifndef OGL_2D_TEXTURE
#define OGL_2D_TEXTURE

#include "Texture.h"
#include <gl\glew.h>

#include <map>
using std::map;

class OGL2DTexture :
	public Texture
{
protected:
	GLuint id;
	GLenum target;
	// Specifies the level-of-detail number. Level 0 is the base image level. 
	// Level n is the nth mipmap reduction image.
	GLint levelOfDetail; 
	GLint internalFormat;
	GLenum texelFormat;
	GLenum typeOfData;
	bool usingMipMaps;
	map<GLenum, GLint> textureParameters;

public:
	OGL2DTexture();

	OGL2DTexture(Texture* texture);

	virtual ~OGL2DTexture();

	GLuint getId() { return this->id; }

	void setTexelFormat(GLenum texelFormat) { this->texelFormat = texelFormat; }

	void setTypeOfData(GLenum typeOfData) { this->typeOfData = typeOfData; }

	void addTextureParameter(GLenum key, GLint value);
	
	void clearTextureParameters() { this->textureParameters.clear(); }

	void setUsingMipMaps(bool usingMipMaps) { this->usingMipMaps = usingMipMaps; }

	void create();

	void select();

protected:
	void applyTextureParameters();
};

#endif

