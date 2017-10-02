#pragma once
#ifndef OGL_2DHUD
#define OGL_2DHUD

#include "OGLGameObject.h"
#include "OGL2DTextDisplay.h"

#include <vector>
using std::vector;

#include <string>
using std::string;

class OGL2DTexture;
class OGL2DTextDisplay;
class OGLShaderProgram;

class OGL2DHUD :
	public OGLGameObject
{
protected:
	OGL2DTexture *texture, *textDisplayTexture;
	OGL2DTextDisplay *flyingHealth, *soldierHealth, *tankHealth;
	GLuint vbo;
	float originX, originY;

public:
	OGL2DHUD(const string& name, float x, float y);

	virtual ~OGL2DHUD();

	void setTexture(OGL2DTexture* texture) { this->texture = texture; }

	void setTextDisplayTexture(OGL2DTexture* textDisplayTexture) { this->textDisplayTexture = textDisplayTexture;  }

	void setTextDisplayShader(OGLShaderProgram *shader);

	OGL2DTextDisplay* getFlyingHealth() { return this->flyingHealth; }

	OGL2DTextDisplay* getSoldierHealth() { return this->soldierHealth; }

	OGL2DTextDisplay* getTankHealth() { return this->tankHealth; }

	void render();

	void renderHUD(float x, float y);

	glm::vec3 getPosition() { return glm::vec3(0, 0, 0); }

};
#endif