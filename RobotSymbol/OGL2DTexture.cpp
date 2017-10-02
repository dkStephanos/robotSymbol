#include "OGL2DTexture.h"

OGL2DTexture::OGL2DTexture() : Texture()
{
	this->id = 0;
	this->target = GL_TEXTURE_2D;
	this->levelOfDetail = 0;
	this->internalFormat = GL_RGB;
	this->texelFormat = GL_RGB;
	this->typeOfData = GL_FLOAT;
	this->addTextureParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	this->addTextureParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	this->usingMipMaps = true;
}

OGL2DTexture::OGL2DTexture(Texture* texture) : OGL2DTexture()
{
	this->set(texture->getData(), texture->getWidth(), texture->getHeight());
}

OGL2DTexture::~OGL2DTexture()
{
}

void OGL2DTexture::addTextureParameter(GLenum key, GLint value)
{
	textureParameters[key] = value;
}

void OGL2DTexture::create()
{
	glGenTextures(1, &this->id);
	glBindTexture(this->target, this->id);
	glTexImage2D(
		this->target,
		this->levelOfDetail,
		this->internalFormat,
		this->width,
		this->height,
		0, // Border, always 0
		this->texelFormat,
		this->typeOfData,
		this->data
	);
	this->applyTextureParameters();
	if (usingMipMaps) {
		glGenerateMipmap(this->target);
	}
}

void OGL2DTexture::select()
{
	glBindTexture(this->target, this->id);
}

void OGL2DTexture::applyTextureParameters()
{
	auto iterator = this->textureParameters.begin();
	while (iterator != this->textureParameters.end()) {
		glTexParameteri(this->target, iterator->first, iterator->second);
		iterator++;
	}

}
